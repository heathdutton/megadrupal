Written by Salvador Molina <salvador.molinamoreno@codeenigma.com>
                           <drupal.org: https://drupal.org/user/826088>


CONTENTS OF THIS FILE
---------------------

 * INTRODUCTION
 * INSTALLATION
 * USAGE
 * ROADMAP


INTRODUCTION
------------

This module allows developers to create interactive Drush commands to request
all sorts of data to the user, in order to easily carry out complex tasks, whose
steps might depend on the data entered in previous steps. The goal of the module
is to provide developers with an easy-to-use tool that they can extend to meet
their needs, in less time than it would take to achieve the same thing using
drush alone.


INSTALLATION
------------

To install the IDC module, follow the standard installation steps for drupal
modules. No extra steps are required.

USAGE
-----

The process to implement an interactive drush command is very straightforward.
At its simplest, each command is just a PHP class implementing the IDCInterface
interface class. Is recommended that your class extends the IDCCommandBase class
too, because it implements most of the interface methods, leaving the command
class to simply hold the command steps definition:

  1. Tell ctools the directory where your commands are placed:

       /**
        * Implements hook_ctools_plugin_directory().
        */
       function {your_module}_ctools_plugin_directory($module, $plugin) {
         if ($module == 'idc' && $plugin == 'idc_commands') {
           return 'plugins/idc_commands';
         }
       }

  2. Each one of your IDC command files has to return a $plugin array specifying
     the name of the class that will execute the command.

      E.G:  $plugin = array(
              'handler class' => 'MyDrushInteractiveCommand',
            );

  3. Create your class:

      E.G: (class MyDrushInteractiveCommand extends IDCCommandBase implements IDCInterface).

  4. Set your command name in the var expected by IDCCommandBase:

      E.G: public static $commandName = 'my-drush-interactive-command';

  5. Implement the static method getCommandDefinition(). It has to return the
     drush command definition, as it would be in hook_drush_command(). Note the
     'callback' property will be overriden by the IDC module to use its own.

  6. Implement the method getCommandSteps(). It just needs to return an array
     with the steps that the command will run. Each name must match a method
     name within the command class. Example:

        public function getCommandSteps() {
          return array(
            'step_1',
            'step_2',
            'step_3',
          );
        }

        Then, your command class should have the methods step_1(), step_2() and
        step_3(). If any method is not present, that step will be ignored.

     6.1. Each of the step methods has to return an object that extends the
          class IDCStepBase, with the question to show to the user, and the
          options available for him. At the moment, there are 2 existing Classes
          available for this: IDCStepSingleChoice, and IDCStepMultipleChoice.

          Example of step method implementation:

            public function step_3($processed = FALSE) {
              // Step label and options.
              $step = new IDCStepMultipleChoice('Hey user, choose something',
               array('option 1', 'option 2', 'option 3');
              return $step;
            }

          The first time the step is executed, $processed is FALSE. After the
          IDCStepMultipleChoice object has been returned, and the user has
          entered some options on the command line, the method is executed again
          with $processed = TRUE, before the execution flow moves on to the next
          method.

  7. Implement the method finishExecution(). It will be called when all the
     steps have been processed, letting you add any other logic required to
     finish the command before the process is actually closed.


ROADMAP
-------

reset_results support:   To allow commands to reset all the results stored so
                         far (for a single step, or for the whole execution).
