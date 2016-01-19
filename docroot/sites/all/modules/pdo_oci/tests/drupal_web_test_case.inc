<?php

require_once dirname(__FILE__) . '/devel.inc';
require_once dirname(__FILE__) . '/simpletest/autorun.php';
require_once dirname(__FILE__) . '/simpletest/reporter.php';

class DrupalWebTestCase extends UnitTestCase {

  function setUp() {
    if (strpos(dirname(__FILE__), 'mysql')) {
      shell_exec("mysql -u root --password='CHANGE' < " . dirname(__FILE__) . "/drupal_web_test_case.mysql.sql");
    }
    elseif (strpos(dirname(__FILE__), 'oci')) {
      shell_exec("sqlplus SYS@AL32UTF8/CHANGE AS SYSDBA @" . dirname(__FILE__) . "/drupal_web_test_case.oci.sql");

      require_once DRUPAL_ROOT . '/includes/install.inc';
      require_once dirname(__FILE__)  . '/../install.inc';

      $task = new DatabaseTasks_oci();
      $task->initializeDatabase();
    }
  }

  public static function randomString($length = 8) {
    $str = '';
    for ($i = 0; $i < $length; $i++) {
      $str .= chr(mt_rand(32, 126));
    }
    return $str;
  }

  public static function randomName($length = 8) {
    $values = array_merge(range(65, 90), range(97, 122), range(48, 57));
    $max = count($values) - 1;
    $str = chr(mt_rand(97, 122));
    for ($i = 1; $i < $length; $i++) {
      $str .= chr($values[mt_rand(0, $max)]);
    }
    return $str;
  }
}
SimpleTest::prefer(new DrupalWebTestCase());

class DrupalHtmlReporter extends HtmlReporter {

  function paintPass($message) {
    parent::paintPass($message);
    print "<span class=\"pass\">Pass</span>: ";
    $breadcrumb = $this->getTestList();
    array_shift($breadcrumb);
    print implode(" -&gt; ", $breadcrumb);
    print " -&gt; " . $this->htmlEntities($message) . "<br />\n";
  }
}
SimpleTest::prefer(new DrupalHtmlReporter());

function database_test_schema() {
  $schema['test'] = array(
    'description' => 'Basic test table for the database unit tests.',
    'fields' => array(
      'id' => array(
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'name' => array(
        'description' => "A person's name",
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'age' => array(
        'description' => "The person's age",
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'default' => 0,
      ),
      'job' => array(
        'description' => "The person's job",
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => 'Undefined',
      ),
    ),
    'primary key' => array('id'),
    'unique keys' => array(
      'name' => array('name')
    ),
    'indexes' => array(
      'ages' => array('age'),
    ),
  );

  // This is an alternate version of the same table that is structured the same
  // but has a non-serial Primary Key.
  $schema['test_people'] = array(
    'description' => 'A duplicate version of the test table, used for additional tests.',
    'fields' => array(
      'name' => array(
        'description' => "A person's name",
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'age' => array(
        'description' => "The person's age",
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'default' => 0,
      ),
      'job' => array(
        'description' => "The person's job",
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
    ),
    'primary key' => array('job'),
    'indexes' => array(
      'ages' => array('age'),
    ),
  );

  $schema['test_one_blob'] = array(
    'description' => 'A simple table including a BLOB field for testing BLOB behavior.',
    'fields' => array(
      'id' => array(
        'description' => 'Simple unique ID.',
        'type' => 'serial',
        'not null' => TRUE,
      ),
      'blob1' => array(
        'description' => 'A BLOB field.',
        'type' => 'blob',
      ),
    ),
    'primary key' => array('id'),
    );

  $schema['test_two_blobs'] = array(
    'description' => 'A simple test table with two BLOB fields.',
    'fields' => array(
      'id' => array(
        'description' => 'Simple unique ID.',
        'type' => 'serial',
        'not null' => TRUE,
      ),
      'blob1' => array(
        'description' => 'A dummy BLOB field.',
        'type' => 'blob',
      ),
      'blob2' => array(
        'description' => 'A second BLOB field.',
        'type' => 'blob'
      ),
    ),
    'primary key' => array('id'),
    );

  $schema['test_task'] = array(
    'description' => 'A task list for people in the test table.',
    'fields' => array(
      'tid' => array(
        'description' => 'Task ID, primary key.',
        'type' => 'serial',
        'not null' => TRUE,
      ),
      'pid' => array(
        'description' => 'The {test_people}.pid, foreign key for the test table.',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'default' => 0,
      ),
      'task' => array(
        'description' => 'The task to be completed.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'priority' => array(
        'description' => 'The priority of the task.',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'default' => 0,
      ),
    ),
    'primary key' => array('tid'),
  );

  $schema['test_null'] = array(
    'description' => 'Basic test table for NULL value handling.',
    'fields' => array(
      'id' => array(
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'name' => array(
        'description' => "A person's name.",
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'default' => '',
      ),
      'age' => array(
        'description' => "The person's age.",
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
        'default' => 0),
    ),
    'primary key' => array('id'),
    'unique keys' => array(
      'name' => array('name')
    ),
    'indexes' => array(
      'ages' => array('age'),
    ),
  );

  $schema['test_serialized'] = array(
    'description' => 'Basic test table for NULL value handling.',
    'fields' => array(
      'id' => array(
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'name' => array(
        'description' => "A person's name.",
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'default' => '',
      ),
      'info' => array(
        'description' => "The person's data in serialized form.",
        'type' => 'blob',
        'serialize' => TRUE,
      ),
    ),
    'primary key' => array('id'),
    'unique keys' => array(
      'name' => array('name')
    ),
  );

  return $schema;
}
