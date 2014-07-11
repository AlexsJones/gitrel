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
#include <stdlib.h>
#include <stdio.h>
#include <git2.h>
#include <jnxc_headers/jnxlog.h>
#include <jnxc_headers/jnxcheck.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
git_commit * get_last_commit ( git_repository * repo )
{
	int rc;
	git_commit * commit = NULL; /*  the result */
	git_oid oid_parent_commit;  /*  the SHA1 for last commit */

	/*  resolve HEAD into a SHA1 */
	rc = git_reference_name_to_id( &oid_parent_commit, repo, "HEAD" );
	if ( rc == 0 )
	{
		/*  get the actual commit structure */
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
	printf("[REMOTES]\n");
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
void list_branches(git_repository *repo, git_branch_t type) {

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
				printf("%d)%s [CURRENT]",c,name);
			}else {
				printf("%d)%s",c,name);
			}

			git_commit *latest = get_last_commit(repo);
			//latest commit id
			const git_oid *oid = git_commit_id(latest);
			char *newsha = git_oid_allocfmt(oid);
			//latest commit message
			const char *message = git_commit_message(latest);
			printf("[%s][%s]",newsha,message ? message : "None");
			git_commit_free(latest);
			free(newsha);
			//latest symbolic target
			const char *symbolic_target = git_reference_symbolic_target(ref);
			if(symbolic_target) {
				printf(" => %s",symbolic_target);
			}
			printf("\n");
		}
		++c;
	}
	if(c == 0) {
		printf("NONE\n");
	}
	git_branch_iterator_free(iterator);
}
void usage() {
	printf("========================gitrel============================\n");
	printf("A simple tool for reminding you what's going on with git\n");
	printf("==========================================================\n");
	exit(0);
}
int main(int argc, char **argv) {

	int c;
	int option_index = 0;
	static struct option long_option[] = { 
		{"help",no_argument, 0, 'h' }
	};
	while(((c = getopt_long(argc, argv,"h",long_option,&option_index))) != -1) {
		switch(c) {
			case 'h':
				usage();
				break;
		}
	}
	char wd[256];
	getcwd(wd,256);	
	if (git_repository_open_ext(NULL,wd, GIT_REPOSITORY_OPEN_NO_SEARCH, NULL) == 0) {
		git_repository *repo = NULL;
		int error = git_repository_open_ext(&repo,wd, GIT_REPOSITORY_OPEN_NO_SEARCH, NULL);
		JNXCHECK(error == 0);
		list_remotes(repo);
		printf("[LOCAL BRANCHES]\n");
		list_branches(repo, GIT_BRANCH_LOCAL);	
		printf("[REMOTE BRANCHES]\n");	
		list_branches(repo, GIT_BRANCH_REMOTE);		
	}else {
		JNX_LOGC(JLOG_CRITICAL,"Not a git repository\n");
	}
	return 0;
}
