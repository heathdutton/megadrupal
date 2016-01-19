Git Rules defines some actions, conditions and events that let's you interact git repositories.

## Actions
Git Rules defines the following actions that can be used to control git repositories:

* Initiate git repositories (git init)
* Clone repositories (git clone)</li>
* Add remotes to repositories (git remote add)
* Remove remotes to repositories (git remote rm)
* Fetch repositories (git fetch)


## Events
Git rules has two events, both are actually triggered at the same time:

* Post receive - Triggered when someone pushes changes into the repository
* Post receive commit - Triggered for each commit that was received in the post receive call.


## Conditions
Git rules has a condition that checks if a repository exists.

## Configuration
Since you probably don't want your web user to write to your repositories,
git rules provides a drush command that executes all actions that have been queued up.
In order to get this working, you need to set up your crontab to run a drush command, git-rules-exec.
This can look something like this:


*/1 * * * * [your-preferred-user] /path/to/drush --root=/path/to/site --uri=uri.to.site git-rules-exec > /dev/null 2>&1


If you want to use the events,
you need to configure your repos to call a drush command on post receive.
Add something like this to /your-repo/hooks/post-receive:


#!/bin/bash
exec /home/nodeone/apps/drush/drush --root=/srv/www/127/web --uri=code.nodeone.se git-rules-post-receive /home/nodeone/projects/test test;
