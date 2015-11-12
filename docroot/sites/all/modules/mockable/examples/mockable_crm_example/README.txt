Mockable CRM Demo
=================

Let's say your module needs to interact via a simplified [REST](http://en.wikipedia.org/wiki/Representational_state_transfer) interace with a CRM maintained by your client.

Your code might look something like this:

    function mymodule_mockable_crm_add($name, $email) {
      if (module_exists('mockable')) {
        return mockable('mymodule_crm_add', $name, $email);
      }
      else {
        return mymodule_crm_add($name, $email);
      }
    }

    function mymodule_mockable_crm_get($record_id) {
      if (module_exists('mockable')) {
        return mockable('mymodule_crm_get', $record_id);
      }
      else {
        return mymodule_crm_get($record_id);
      }
    }
    ...
    function mymodule_crm_add($name, $email) {
      // interact with your CRM;
      return $record_id;
    }
    ...
    function mymodule_crm_get($record_id) {
      // interact with your CRM;
      return $record;
    }
    ...
    function mymodule_crm_add_mock($name, $email) {
      // works only if 'mymodule_crm_*' has been set to mock
      $database = variable_get('mymodule_mock_crm', array());
      $database[] = array($name, $email);
      variable_set($database);
      return count($database) - 1;
    }

    function mymodule_crm_get_mock($record_id) {
      $database = variable_get('mymodule_mock_crm', array());
      return isset($database[$record_id])?$database[$record_id]:NULL;
    }

Now whenever you want to interact with your CRM within your code, you can call:

    ...
    $record_id = mymodule_mockable_crm_add($name, $email);
    ...
    mymodule_mockable_crm_get($record_id);
    ...

When you want to use the mock CRM, in drush you can call:

    drush mockable-set mymodule_crm_*

Or, within your Simpletest, you can call

    mockable_set('mymodule_crm_*');

Using objects, another implementation of the above might be:

    function mymodule_mockable_crm_add($name, $email) {
      mymodule_mockable_crm()->add($name, $email);
    }

    function mymodule_mockable_crm_get($record_id) {
      mymodule_mockable_crm()->get($record_id);
    }

    function mymodule_mockable_crm() {
      if (module_exists('mockable')) {
        return mockable('mymodule_crm');
      }
      else {
        return mymodule_crm();
      }
    }

    function mymodule_crm() {
      static $object;
      if (!$object) {
        $object = new MyModuleRealCRM;
      }
      return object;
    }

    function mymodule_crm_mock() {
      static $object;
      if (!$object) {
        $object = new MyModuleMockCRM;
      }
      return object;
    }

    abstract class MyModuleCRMBase {
      abstract function add($name, $email);
      abstract function get($record_id);
    }

    class MyModuleRealCRM {
      function add($name, $email) {
        // real code here
      }
      function get($record_id) {
        // real code here
      }
    }

    class MyModuleMockCRM {
      function add($name, $email) {
        // mock code here
      }
      function get($record_id) {
        // mock code here
      }
    }
