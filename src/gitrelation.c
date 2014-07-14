/*
 * =====================================================================================
 *
 *       Filename:  gitrelation.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/10/14 17:58:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

#include <stdlib.h>
#include <stdio.h>
#include <git2.h>
#include <jnxc_headers/jnxlog.h>
#include <jnxc_headers/jnxcheck.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define NON_VERBOSE_MAX_DISPLAY 5

git_commit * get_last_commit ( git_repository * repo, const char *symb)
{
	int rc;
	git_commit * commit = NULL; /*  the result */
	git_oid oid_parent_commit;  /*  the SHA1 for last commit */
	rc = git_reference_name_to_id( &oid_parent_commit, repo, symb);
	if ( rc == 0 )
	{
		rc = git_commit_lookup( &commit, repo, &oid_parent_commit );
		if ( rc == 0 )
		{
			return commit;
		}
	}
	return NULL;
}
void list_remotes(git_repository *repo) {
	git_strarray remotes = {0};
	int error = git_remote_list(&remotes, repo);
	int x;
	for(x=0;x<remotes.count;++x) {
		printf("%s ",remotes.strings[x]);
		git_remote *remote = NULL;
		error = git_remote_load(&remote,repo,remotes.strings[x]);
		if(!error) {
			printf("%s\n",git_remote_url(remote));
		}
	}
	if(x == 0) {
		printf("NONE\n");
	}
	git_strarray_free(&remotes);
}
void list_branches(git_repository *repo, git_branch_t type, int isverbose) {

	git_reference *head_ref;
	int error = git_repository_head(&head_ref,repo);
	JNXCHECK(error == 0);
	const char *head_name;
	error =git_branch_name(&head_name,head_ref);
	JNXCHECK(error == 0);
	git_branch_iterator *iterator;
	error = git_branch_iterator_new(&iterator,repo,type);
	if(error != 0) {
		JNX_LOGC(JLOG_CRITICAL,"Could not create local branch iterator\n");
	}
	git_reference *ref;
	int c = 0;
	while(git_branch_next(&ref,&type,iterator) != GIT_ITEROVER) {
		const char *name;
		if(git_branch_name(&name,ref) == 0) {
			if(strcmp(name,head_name) == 0) {
				printf("%d)%s \x1B[34m[CURRENT]\033[0m",c,name);
			}else {
				printf("%d)%s",c,name);
			}
			printf("\n");
		}
		++c;
		if(!isverbose && c >= NON_VERBOSE_MAX_DISPLAY) {
			break;
		}
	}
	if(c == 0) {
		printf("NONE\n");
	}
	git_branch_iterator_free(iterator);
}
void list_all_references(git_repository *repo, int isverbose) {
	git_strarray ref_list;
	const char *refname;
	git_reference *ref;
	int i;
	char out[41];
	out[40]	= '\0';
	if(git_reference_list(&ref_list,repo) == 0) {
		for (i = 0; i < ref_list.count; ++i) {
			refname = ref_list.strings[i];
			git_reference_lookup(&ref, repo, refname);
			switch (git_reference_type(ref)) {
				case GIT_REF_OID:
					git_oid_fmt(out, git_reference_target(ref));
					printf("%s => [%s]\n", refname, out);
					break;
				case GIT_REF_SYMBOLIC:
					printf("%s => %s\n", refname, git_reference_symbolic_target(ref));
					break;
				default:
					fprintf(stderr, "Unexpected reference type\n");

			}

			if(!isverbose && i >= NON_VERBOSE_MAX_DISPLAY) {
				break;
			}
		}
		git_strarray_free(&ref_list);

	}
}
void usage() {
	printf("========================gitrel============================\n");
	printf("A simple tool for reminding you what's going on with git\n");
	printf("Toggle --verbose for full output\n");
	printf("==========================================================\n");
	exit(0);
}
int main(int argc, char **argv) {

	int c;
	int option_index = 0;
	int v = 0;
	static struct option long_option[] = { 
		{"help",no_argument, 0, 'h' },
		{"verbose",no_argument,0,'v'}
	};
	while(((c = getopt_long(argc, argv,"h",long_option,&option_index))) != -1) {
		switch(c) {
			case 'h':
				usage();
				break;
			case 'v':
				v = 1;
				break;
		}
	}
	char wd[256];
	getcwd(wd,256);	
	if (git_repository_open_ext(NULL,wd, GIT_REPOSITORY_OPEN_NO_SEARCH, NULL) == 0) {
		git_repository *repo = NULL;
		int error = git_repository_open_ext(&repo,wd, GIT_REPOSITORY_OPEN_NO_SEARCH, NULL);
		printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
		JNXCHECK(error == 0);
		printf("%s[REMOTES]%s\n",KYEL,RESET);
		list_remotes(repo);
		printf("%s[REFS]%s\n",KYEL,RESET);
		list_all_references(repo,v);
		printf("%s[LOCAL BRANCHES]%s\n",KYEL,RESET);
		list_branches(repo, GIT_BRANCH_LOCAL,v);	
		printf("%s[REMOTE BRANCHES]%s\n",KYEL,RESET);	
		list_branches(repo, GIT_BRANCH_REMOTE,v);		
		printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");

	}else {
		JNX_LOGC(JLOG_CRITICAL,"Not a git repository\n");
	}
	return 0;
}
