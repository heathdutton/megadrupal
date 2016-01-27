The moudle provdes couple of capabilities

_________________________________________________
Capability 1: Map custom events to flow templates
_________________________________________________ 
The module provide a hook called hook_maestro_enabled_processes see maestro_initiator.api.php.
e.g.
/**
 * Implements hook_maestro_enabled_processes().
 */
function flowers_maestro_enabled_processes() {
  return array('flowers' => array(
      'title' => t('Flowers'), 
      'processes' => array(
        'flowers_oncreate' => array('title' => t('Creation of Flowers')),
        'flowers_onedit' => array('title' => t('Edit of Flowers')),
        'flowers_ondelete' => array('title' => t('Delete of Flowers')),
      ),
    )
  );
}


Define the events for which the workflow can be initiated. Users will be able to change templates
using the Admin -> Structure -> Maestro Workflows -> Manage Templates.

You can initiate a workflow using the api

maestro_initiator_initiate_workflow('key', $params);
e.g.

maestro_initiator_initiate_workflow('flowers_oncreate', array('flowerid' => $fid));

_________________________________________________
Capability 2: Hold Task.
_________________________________________________
The hold task provides the capability to automatically complete a flow after a certain time period.
Hold Task uses two variables

hold_task: specifies the number days by which the sibling flows must be completed from the date of creation.
If the sibling flow steps are not completed then this will automatically complete the flows.

hold_until: specify in mm/dd/yyyy or numeric (result of strtotime()) format until which the sibiling
tasks must be executed. If this variable is set it has a higher preference over hold_task

When tasks are autocompleted, the task hander is called with autocomplete = TRUE in the params argument 
of the task handler.

_________________________________________________
Capability 3: Generic form
_________________________________________________
A generic form (maestro_initiator_general_form) to be shown in an interactive form. you can 
form elements to $params and they will be shown in the task console for the task.

_________________________________________________
Capability 4: Userbar Integration
_________________________________________________
This provides the capability to integrate with http://drupal.org/project/userbar module.

