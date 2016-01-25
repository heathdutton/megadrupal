Drush Drake
===========

Yet another site-building/rebuilding/stuff-doing tool for Drush, this
one taking much more inspiration from the likes of make(1) (and rake,
pake, phake, ant, phing, etc), and other more or less general purpose
building tools, but with Drush Flavour (TM).

Intro
=====

Drake works sorta like make by having a drakefile.php file which tells
it what to do. The drakefile.php may be in the current directory or in
sites/all/drush of the current site.

A drakefile.php file looks a lot like a Drush aliases.drushrc.php
file, and contains tasks and, optionally, actions, in the variables
$tasks and $actions, respectively.

An action is the implementation of something to do, such as syncing a
database or running a Drush command. At the moment the selection is
rather bare, but the default set of running shell, drush and drake
commands should go a long way.

Tasks specify which actions you want to run, how, and in which
order. Tasks is much like the targets of traditional make(1), as
they're the main thing you'll be using on a daily basis.

When invoking Drake without any arguments, it will run the first
task found in the file. Providing the name of a task will run that
specific task.

A task can be dependent on other tasks by using the 'depends' key,
which will make Drake run the depended upon tasks before running
the given task.

An example:

<?php

$tasks['default'] = array(
  'depends' => array('first-task', 'second-task'),
  'action' => 'shell',
  'command' => 'echo "first-task and second-task is now done."',
);

$tasks['first-task'] = array(
  'action' => 'shell',
  'command' => 'echo "First task!"',
);

$tasks['second-task'] = array(
  'action' => 'drush',
  'command' => 'php-eval',
  'args' => array('drush_print("Second task!")'),
);

?>

With the above drakefile.php, running drush drake with no arguments
will run the tasks depended on by the default task before running the
default task itself.

Drake file discovery
====================

If no --file argument is specified, Drake attempts to find a drakefile
by looking in the following places:

When run in a Drupal install: sites/all/drush/drakefile.php

A drakefile.php in the current directory.

When run in a Drupal install: A drakefile.php in any profile in
the profiles folder (but only if just one is found in all profiles).

In any case, any *.drakefile.drushrc.php files found in any directory
in the users .drush folder or sites/all/drush is included before
loading the main drakefile.php, but no default tasks is run in those.

Actions
=======

Custom actions can be defined in $actions of a drakefile. The simplest
case looks like this:

<?php

$actions['my-action'] = array(
  'callback' => 'action_callback',
);
function action_callback($context) {
  // Do something.
}

?>

The context passed to action callbacks is the task being run. An
action can define what task properties it requires or uses by
populating the parameters key in $actions (see drush.actions.inc for
examples).

Drake has the following default actions:

'shell':
  Execute shell commands, parameters:
  'command': the command to run.
  'args':    command arguments, optional. 

'drush':
  Execute drush commands, parameters:
  'command': the command to run.
  'args':    command arguments, optional.
  'target':  Drush alias to run the command on, defaults to @self.

'drake':
  Run drake, parameters:
  'file':    drakefile to use.
  'dir':     Directory to run in, optional.
  'task':    Task to run, optional.
  'target':  Drush alias to run the command on, defaults to @self.

Contexts
========

As running tasks with hard coded values rather limits the reusability of
tasks, we have contexts. They allows you to pass arguments to tasks, and follows the dependency chain. 

<?php

$tasks['echo-task'] = array(
  'action' => 'shell',
  'command' => 'echo',
  'args' => array(context('echo-string')),
);

$tasks['do-echo'] = array(
  'depends' => 'echo-task',
  'context' => array(
    'echo-string' => 'Hello world!',
  ),
);

?>

When the echo-task runs as a dependency of do-echo, the echo-string
context is set to "Hello world!". If do-echo didn't have that context,
the file context of the file that contained the echo-task would be
checked (these can be defined by having a $context variable in the
drakefile), before checking the global context.

Drake sets the context @self to the name of the alias of the current
site (if any).

Contexts may also specify an format for the string, for example:

<?php
  'args' => context('--file=[filename]'),
?>

will replace the `filename` context into the string, and use that as
args.

A context may also be declared optional by using context_optional
instead, which takes another, optional, argument which is the default
value. If the default argument is not used, or it is null, the
property will be removed if the context is not available. For example:

<?php
  'args' => context_optional('--file=[filename]'),
?>

Will remove the `args` key if the context was not found.

Context Ops
-----------

As static strings is somewhat limited in their usage, contexts (and
arguments, more on those later) supports working with the context
value by using "ops". A practical example:

  'path' => context('[@prod:site:root]'),

This will set the path context to the root path of the @prod
alias. Ops works from left to right, in the above case starting with
the @prod context, converting it to a site op, which we can then ask
for the root of the site. Most ops evaluate to the original string
they were based on, so "[@prod:site]" is basically the same as
"[@prod]", but the next op allows for further refinement.

Another example:

  'path' => context('[@prod:site:root:file:dirname]'),

Gets the parent directory name of the site root.

Defined Ops
-----------
The currently defined ops are:

site:         The original string is taken as a site alias.
  root:       The root of the Drupal installation.
  path:       The path within the root for the site.

string:       Misc string functions.
  upcase:     Convert string to uppercase.
  downcase:   Convert string to lowercase.

file:         Takes a filename.
  dirname:    Parent directory name of the file.
  basename:   Filename without the directory name.

Arguments
=========

Arguments is basically a special case of contexts. When Drake
initializes, it takes any additional command line arguments, and turns
them into contexts. Arguments of the form `name=value` sets a named
context to the given value, while those that does not contain any
un-escaped equals signs is provided as contexts named arg1, arg2, etc.

When using arguments, you should use drake_argument() (or
drake_argument_optional()), instead of context(), as it will provide
the user with more informal error messages. An example:

<?php

$tasks['echo-task'] = array(
  'action' => 'shell',
  'command' => 'echo',
  'args' => array(drake_argument(1, 'the string to echo')),
);

?>

Will take the first argument on the command line not of the key=val
form and echo it.
