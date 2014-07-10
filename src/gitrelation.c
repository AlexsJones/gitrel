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
#include <git2.h>
#include <jnxc_headers/jnxlog.h>
#include <unistd.h>

int main(int argc, char **argv) {

	char wd[256];
	getcwd(wd,256);	
	JNX_LOGC(JLOG_NORMAL,"Directory => %s\n",wd);	
	if (git_repository_open_ext(NULL,wd, GIT_REPOSITORY_OPEN_NO_SEARCH, NULL) == 0) {
		git_repository *repo = NULL;
		int error = git_repository_open_ext(&repo,wd, GIT_REPOSITORY_OPEN_NO_SEARCH, NULL);
		JNX_LOGC(JLOG_NORMAL,"git_repository_open_ext returned %d\n",error);

	}else {
		JNX_LOGC(JLOG_CRITICAL,"Not a git repository\n");
	}
	return 0;
}
