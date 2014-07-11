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
#include <unistd.h>


void list_remotes(git_repository *repo) {
	git_strarray remotes = {0};
	int error = git_remote_list(&remotes, repo);
	int x;
	printf("-Remotes->\n");
	for(x=0;x<remotes.count;++x) {
		printf("-",remotes.strings[x]);
		git_remote *remote = NULL;
		error = git_remote_load(&remote,repo,remotes.strings[x]);
		if(!error) {
			printf("%s\n",git_remote_url(remote));
		}
	}
	git_strarray_free(&remotes);
}
void list_branches(git_repository *repo, git_branch_t type) {

	git_branch_iterator *iterator;
	int error = git_branch_iterator_new(&iterator,repo,type);
	if(error != 0) {
		JNX_LOGC(JLOG_CRITICAL,"Could not create local branch iterator\n");
	}
	git_reference *ref;
	int c = 0;
	while(git_branch_next(&ref,&type,iterator) != GIT_ITEROVER) {
		const char *name;
		if(git_branch_name(&name,ref) == 0) {
			printf("%d)%s\n",c,name);
		}
		++c;
	}
	git_branch_iterator_free(iterator);
}
int main(int argc, char **argv) {

	char wd[256];
	getcwd(wd,256);	
	printf("Directory => %s\n",wd);	
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
