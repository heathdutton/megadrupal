<?php 
/**
 * GOTCHAs
 *
 * - $base_url in settings.php must be set if site isn't at http://localhost/
 */

// TODO Don't hardcode path to features but use path specified in behat.yml
define('FEATURES_PATH', getcwd() . '/features/');

use Behat\Behat\Context\ClosuredContextInterface as ClosuredContext,
    Behat\Behat\Context\TranslatedContextInterface as TranslatedContext,
    Behat\Behat\Context\BehatContext;

use Symfony\Component\Finder\Finder;

if (file_exists(__DIR__ . '/../support/bootstrap.php')) {
    require_once __DIR__ . '/../support/bootstrap.php';
}

class BehatWebTestCase extends DrupalWebTestCase {
  public $users = array();
  
  public function __call($name, array $args) {
    // Lets us to access protected methods of DrupalWebTestCase
    return call_user_func_array(array($this, $name), $args);
  }
  
  function setUp($feature) {
    require_once(FEATURES_PATH . 'setup.php');
    $setup = behat_feature_setup($feature);
    
    parent::setUp($setup['modules']);
  }
  
  function tearDown() {
    parent::tearDown();
  }
  
  /**
   * Implement our own assert function.
   * Failed assertions need to throw an exception so behat will handle them.
   */
  protected function assert($status, $message = '', $group = 'Other', array $caller = NULL) {
    // copied from DrupalTestCase::assert()
    
    // Convert boolean status to string status.
    if (is_bool($status)) {
      $status = $status ? 'pass' : 'fail';
    }
    
    // Make any step with a failing assertion stop the scenario
    // from continuing.
    if ($status == 'fail') {
      throw new Exception('failedAssertion: '. $message);
    }

    // Increment summary result counter.
    $this->results['#' . $status]++;

    // Get the function information about the call to the assertion method.
    if (!$caller) {
      $caller = $this->getAssertionCall();
    }

    // Creation assertion array that can be displayed while tests are running.
    $this->assertions[] = $assertion = array(
      'test_id' => $this->testId,
      'test_class' => get_class($this),
      'status' => $status,
      'message' => $message,
      'message_group' => $group,
      'function' => $caller['function'],
      'line' => $caller['line'],
      'file' => $caller['file'],
    );

    // Store assertion for display after the test has completed.
    Database::getConnection('default', 'simpletest_original_default')
      ->insert('simpletest')
      ->fields($assertion)
      ->execute();

    // We do not use a ternary operator here to allow a breakpoint on
    // test failure.
    if ($status == 'pass') {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  
  /**
   * Pass if the current relative page path is the given path.
   *
   * @param $path
   *   The string the path should be.
   * @param $message
   *   Message to display.
   * @param $group
   *   The group this message belongs to.
   * @return
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertPath($path, $message = '', $group = 'Other') {
    global $base_url;
    $actual = str_replace($base_url . '/', '', $this->getUrl());
    if (!$message) {
      $message = t('Page path @actual is equal to @expected.', array(
        '@actual' => var_export($actual, TRUE),
        '@expected' => var_export($path, TRUE),
      ));
    }
    return $this->assertEqual($actual, $path, $message, $group);
  }  
  /**
   * Custom Simpletest API methods
   */
  protected function drupalCreateNamedUser($name, $pass, $permissions = array('access comments', 'access content', 'post comments', 'skip comment approval')) {
    // Create a role with the given permission set.
    if (!($rid = $this->drupalCreateRole($permissions))) {
      return FALSE;
    }

    // Create a user assigned to that role.
    $edit = array();
    $edit['name']   = $name;
    $edit['mail']   = $edit['name'] . '@example.com';
    $edit['roles']  = array($rid => $rid);
    $edit['pass']   = $pass;
    $edit['status'] = 1;

    $account = user_save(drupal_anonymous_user(), $edit);

    $this->assertTrue(!empty($account->uid), t('User created with name %name and pass %pass', array('%name' => $edit['name'], '%pass' => $edit['pass'])), t('User login'));
    if (empty($account->uid)) {
      return FALSE;
    }

    // Add the raw password so that we can log in as this user.
    $account->pass_raw = $edit['pass'];
    return $account;
  }
}

/**
 * Feature context with closure definitions.
 */
class ClosuredFeatureContext extends BehatContext implements ClosuredContext, TranslatedContext
{
    /**
     * Environment parameters.
     *
     * @var     array
     */
    public $parameters = array();

    /**
     * @var     object
     */
    public $d = NULL;

    /**
     * Initializes context.
     *
     * @param   array   $parameters
     */
    public function __construct(array $parameters) {
        $this->parameters = $parameters;

        if (file_exists(__DIR__ . '/../support/env.php')) {
            $world = $this;
            require(__DIR__ . '/../support/env.php');
        }
    }

    /**
     * Returns list of step definition files.
     *
     * @see     Behat\Behat\Context\ClosuredContextInterface::getStepDefinitionResources()
     *
     * @return  array
     */
    public function getStepDefinitionResources() {
      // find and return all *.php files under features/steps folder
      $steps_path = FEATURES_PATH . 'steps';
      if (file_exists($steps_path)) {
        $finder = new Finder();
        return $finder->files()->name('*.php')->in($steps_path);
      }
      return array();
    }

    /**
     * Returns list of hook definition files.
     *
     * @see     Behat\Behat\Context\ClosuredContextInterface::getHookDefinitionResources()
     *
     * @return  array
     */
    public function getHookDefinitionResources() {
        // return array of features/support/hooks.php if it exists
        if (file_exists(__DIR__ . '/../support/hooks.php')) {
            return array(__DIR__ . '/../support/hooks.php');
        }
        return array();
    }

    /**
     * Returns list of XLIFF translation files.
     *
     * @see     Behat\Behat\Context\TranslatedContextInterface::getTranslationResources()
     *
     * @return  array
     */
    public function getTranslationResources() {
        // find and return all *.xliff files under features/steps/i18n folder
        if (file_exists(__DIR__ . '/../steps/i18n')) {
            $finder = new Finder();
            return $finder->files()->name('*.xliff')->in(__DIR__ . '/../steps/i18n');
        }
        return array();
    }

    /**
     * Calls previously saved on $world closure function.
     *
     * @param   string  $name   function name
     * @param   array   $args   function args
     *
     * @return  mixed
     */
    public function __call($name, array $args) {        
        if(isset($this->d->$name) && is_callable($this->d->$name)) {
           return call_user_func_array($this->d->$name, $args);
         }

        if (isset($this->$name) && is_callable($this->$name)) {
            return call_user_func_array($this->$name, $args);
        } else {
            $trace = debug_backtrace();
            trigger_error(
                'Call to undefined method ' . get_class($this) . '::' . $name .
                ' in ' . $trace[0]['file'] .
                ' on line ' . $trace[0]['line'],
                E_USER_ERROR
            );
        }
    }
}
