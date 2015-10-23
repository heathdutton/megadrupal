<?php
// $Id$

/**
 *
 * Form builder for the system settings form.
 */
function paypal_api_system_settings() {
  $settings = variable_get('paypal_api_settings', array());
  $form = array(
    '#type'	        => 'fieldset',
    '#collapsible'	=> FALSE,
    '#title'	      => t("Currently configured content types"),
    'help'		=> array(
      '#markup'	=> "
      	<div class='messages'>
      		<p>" . t("The currently configured content types are listed below.  Click on the content type name to edit settings for that content type.") . "</p>
      		<p>" . t("If a content type doesn't appear here, you need to activate it in the !link.", array('!link' => l(t("content type form"), "admin/structure/types"))) . "</p>
      	</div>
      "
    ),
    'types'	=> array(
      '#type'	        => 'fieldset',
      '#title'				=> t("Content types"),
    	'#collapsible'	=> FALSE
    )
  );

  if (!isset($settings['types']) || !$settings['types']) {
    $form['types']['help'] = array(
      '#markup'	=> "<p>" . t("No content types have been activated for Paypal API yet.") . "</p>"
    );
  }
  else {
    foreach ($settings['types'] as $type => $config) {
      $form['types'][$type] = array(
        '#markup'		=> '<p>' . l($type, "admin/settings/paypal_api/" . $type) . '</p>'
      );
    }
  }

  $form['settings'] = array(
    '#type'	        => 'fieldset',
    '#collapsible'	=> (count($settings) < 1),
    '#title'	      => t("Paypal API settings"),
    'email' => array(
      '#type'		        => 'textfield',
      '#title'	        => t("Your Paypal account email"),
      '#description'	  => t("Enter your default Paypal account email address, if you plan on having Paypal API create buttons on the fly for purchase transactions.  You may override this account for individual content types or nodes.  Leave blank if you will only use buttons you create through your account control panel on Paypal."),
      '#default_value'	=> (isset($settings['email']))? $settings['email'] : ''
    ),
    'form_settings' => array(
      '#type'		      => 'fieldset',
      '#title'	      => t("Payment handling"),
      '#collapsible'	=> FALSE,
      'replace_403' => array(
        '#type'		        => 'checkbox',
        '#title'	        => t("Replace 403 Access Denied page?"),
        '#default_value'	=> (isset($settings['replace_403']))? $settings['replace_403'] : '',
        '#description'	=> "
        	<p>" . t("When payment is required but the current user hasn't paid, a 403 Access Denied error will be generated.  With this box checked, Paypal API will use it's own page to handle access denied requests, including those requiring payment.") . "</p>
        	<p><strong>" . t("ATTENTION: Paypal API's page will be used for ALL access denied errors, and will conflict with other modules that use this function, such as Login Toboggan") . "</p>
        	<p>" . t("Alternatively, you may leave this unchecked, and implement hook_paypal_api_access_denied() to customize how access denied errors are handled.") . "</p>
        "
      ),
      'pay_link_message' => array(
        '#type'		        => 'checkbox',
        '#title'	        => t("Add payment link in a status message?"),
        '#default_value'	=> (isset($settings['pay_link_message']))? $settings['pay_link_message'] : '',
        '#description'	=> "
        	<p>" . t("Check this box to cause a message to appear whenever payment is required, containing a link to the payment page.") . "</p>
        "
      )
    ),
    'create_user'	=> array(
      '#type'		        => 'checkbox',
      '#title'	        => t("Allow Paypal API to automatically create user accounts"),
      '#default_value'	=> (isset($settings['create_user']))? $settings['create_user'] : '',
      '#description'	=> "
        <p>" . t("Paypal API relies on users being logged in for managing payments and tracking transactions.  It is a good practice to make sure that users carrying out financial transactions on your site are known to you in some way.") . "</p>
        <p>" . t("If this box is checked, when an anonymous user attempts to pay for content, a user account will be created automatically for them once their payment has gone through.  The user will be assigned to the 'authenticated' role.") . "</p>
      "
    ),
    'notify_user'	=> array(
      '#type'		        => 'checkbox',
      '#title'	        => t("Notify user when account created"),
      '#default_value'	=> (isset($settings['notify_user']))? $settings['notify_user'] : TRUE,
      '#description'	=> "
        <p>" . t("When Paypal API creates a new user account a system email will be sent to the user notifying them of the newly created account if this is checked.") . "</p>
      "
    ),
    'developer_settings' => array(
      '#type'		      => 'fieldset',
      '#title'	      => t("Developer settings"),
      '#collapsible'	=> TRUE,
      '#collapsed'    => TRUE,

      'sandbox'	=> array(
        '#type'		        => 'checkbox',
        '#title'	        => t("Use the Paypal sandbox"),
        '#default_value'	=> (isset($settings['sandbox']))? $settings['sandbox'] : '',
        '#description'	=> "
          <p>" . t("Use the Paypal sandbox to test the system.  This requires that you create a developer account and use buttons you've created inside the sandbox for testing.") . "</p>
        "
      ),
      'domain'	=> array(
        '#type'		        => 'textfield',
        '#title'	        => t("Local domain name"),
        '#default_value'	=> (isset($settings['domain']))? $settings['domain'] : '',
        '#description'	=> "
          <p>" . t("Specify the domain name to use for IPN callbacks, success and cancellation pages.  Leave this blank to use your server's domain '!server_name'.  You should only need to set this if you are behind a firewall or have port forwarding set up to a test machine.", array('!server_name' => $_SERVER['SERVER_NAME'])) . "</p>
        "
      )
    )
  );

  $form['submit'] = array(
    '#type'		=> 'submit',
    '#value'	=> t("Save these settings")
  );

  return $form;
}

/**
 *
 * Submit handler for the system settings form
 *
 * @param $form
 * @param $form_values
 */
function paypal_api_system_settings_submit($form, &$form_values) {
  $settings = variable_get('paypal_api_settings', array());

  foreach (array('email', 'replace_403', 'sandbox', 'domain', 'create_user', 'notify_user', 'pay_link_message') as $field) {
    $settings[$field] = $form_values['values'][$field];
  }
  variable_set('paypal_api_settings', $settings);

  if ($settings['replace_403']) {
    variable_set('site_403', 'paypal/payment-required');
  }

  drupal_set_message(t("Paypal API settings have been updated."), 'status');
}

/**
 *
 * Form builder for the content type settings form.
 *
 * @param $form
 *
 * @param $form_state
 *
 * @param $bundle
 * The content type to set settings for
 */
function paypal_api_type_settings($form, $form_state, $bundle) {
  $config = paypal_api_bundle_config($bundle);
  $form     = array();
  $bundles  = field_info_bundles('node');
  $bundle_info = (isset($bundles[$bundle]))? $bundles[$bundle] : NULL;

  if (!$config) {
    $form['help'] = array(
      '#markup'	=> t("This content type has not been activated for Paypal payments.  !link to activate this content now.", array('!link' => l(t("Edit this content type"), 'admin/structure/types/manage/' . $bundle)))
    );

    return $form;
  }

  $form['type'] = array(
    '#type'		=> 'hidden',
    '#value'  => $bundle
  );

  $form['node_config'] = array(
    '#markup' => '<h2>' . t("Content type '@bundle' configuration", array('@bundle' => $bundle)) . '</h2>'
  );

  $form['operations'] = array(
    '#type'         => 'fieldset',
    '#title'        => t("Operations"),
    '#collapsible'  => FALSE,

    'view' => array(
      '#type'		  => 'checkbox',
      '#title'	  => t("View any '@bundle' node", array('@bundle' => $bundle)),
      '#default_value'	=> (isset($config['view']))? $config['view'] : array(),
      '#description'    => t("The user will be able to view ALL nodes of the '@bundle' content type after payment.  The user may view the nodes as many times as indicated by the Quantity field below.  To allow unlimited viewing, insert the number zero in the Quantity field.", array('@bundle' => $bundle))
    ),
    'instance' => array(
      '#type'		  => 'checkbox',
      '#title'	  => t("View a specific node."),
      '#default_value'	=> (isset($config['instance']))? $config['instance'] : array(),
      '#description'    => t("The user will be able to view one single specified node after payment.  The user may view the node as many times as indicated by the Quantity field below.  To allow unlimited viewing, insert the number zero in the Quantity field.")
    ),
    'create' => array(
      '#type'		  => 'checkbox',
      '#title'	  => t("Create"),
      '#default_value'	=> (isset($config['create']))? $config['create'] : array(),
      '#description'    => t("The user will be able to create new '@bundle' content after paying the set amount.  The number of new nodes the user will be allowed to create can be set using the Quantity field below.  For instance, to allow a user to create one new node for each payment, insert the number one in the Quantity field.", array('@bundle' => $bundle))
    ),
    'deferred_create' => array(
      '#type'		        => 'checkbox',
      '#title'	        => t("Deferred create"),
      '#default_value'	=> (isset($config['deferred_create']))? $config['deferred_create'] : array(),
      '#description'    => t("The user will be allowed to create a new '@bundle' node before payment.  The node's status will be set to UNPUBLISHED until payment has been made, at which time the node will be automatically published.", array('@bundle' => $bundle))
    ),
    'help' => array(
      '#markup' => t("Indicate the operation you want the user to pay for.  You can control how many times this operation may be performed per purchase by filling in the quantity field below.")
    )
  );

  $form['quantity'] = array(
    '#type'		        => 'textfield',
    '#required'	      => TRUE,
    '#title'	        => t("Quantity"),
    '#default_value'	=> (isset($config['quantity']))? $config['quantity'] : 1,
    '#size'					  => 10,
    '#description'    => t("The quantity purchased per transaction.  Use the number zero to indicate an unlimited supply (for instance, a one time purchase allows you to view a given node an unlimited amount of times).  Otherwise, put a number here.")
  );

  $form['paypal_config'] = array(
    '#markup' => '<h2>' . t("Paypal configuration") . '</h2>'
  );


  $form['method'] = array(
    '#type'		  => 'radios',
    '#title'	  => t("Button generation method"),
    '#options'	=> array(
      'paypal'	=> t("You copy and paste button code from your Paypal control panel"),
      'api'			=> t("The Paypal API module generates buttons on the fly according to the configuration you create")
    ),
    '#default_value'	=> (isset($config['method']))? $config['method'] : ''
  );


  $form['paypal'] = array(
    '#type'		      => 'fieldset',
    '#title'	      => t("Paste your button code"),
    '#collapsible'	=> TRUE,
    '#collapsed'	  => (!isset($config['method']) || $config['method'] == 'api'),
		'#states' => array(
      // Show the settings when the method type is 'paypal'
      'visible' => array(
      	'input[name=method]' => array('value' => 'paypal'),
      ),
      'expanded' => array(
      	'input[name=method]' => array('value' => 'paypal'),
      )
    ),
  	'button'	=> array(
      '#type'		        => 'textarea',
      '#title'					=> t("Button code"),
      '#default_value'	=> (isset($config['button']))? $config['button'] : '',
      '#description'		=> "
      	<p>" . t("Paste your button code here if you want people to pay to create or view any node of this content type.  If you want people to pay to view a single node, you must paste your button code in that node's edit form.") . "</p>
      	<p>" . t("Use this content type (!type) as the Item ID of the button to ensure proper functioning of the IPN system.", array('!type' => $bundle)) . "</p>
     	"
    )
  );

  $paypal_docs_url = "https://cms.paypal.com/us/cgi-bin/?cmd=_render-content&content_ID=developer/e_howto_html_Appx_websitestandard_htmlvariables";
  $paypal_link = l(t("See Paypal documentation for a description of this variable."), $paypal_docs_url);

  $form['api'] = array(
    '#type'		=> 'fieldset',
    '#title'	=> t("Configure payment options"),
    '#collapsible'	=> TRUE,
    '#collapsed'	  =>  (!isset($config['method']) || $config['method'] == 'paypal'),
		'#states' => array(
      // Show the settings when the method type is 'api'
      'visible' => array(
      	'input[name=method]' => array('value' => 'api'),
      ),
      'expanded' => array(
      	'input[name=method]' => array('value' => 'api'),
      )
    ),
    'cost'	=> array(
      '#type'		        => 'textfield',
      '#title'					=> t("Default cost"),
      '#default_value'	=> (isset($config['cost']))? $config['cost'] : '',
      '#description'		=> t("Insert the default cost for this item.  You may implement hook_paypal_api_prepay() to override this value at the time of transaction.")
    ),
  	'currency'	=> array(
      '#type'		        => 'select',
      '#title'					=> t("Default currency"),
      '#default_value'	=> (isset($config['currency']))? $config['currency'] : '',
      '#options'				=> paypal_api_currencies(),
      '#description'		=> t("Insert the default currency for this item.  You may implement hook_paypal_api_prepay() to override this value at the time of transaction.")
    ),
  	'item_name'	=> array(
      '#type'		        => 'textfield',
      '#title'					=> t("Item name"),
      '#default_value'	=> (isset($config['item_name']))? $config['item_name'] : (isset($bundle_info)? $bundle_info['label'] : ''),
      '#description'		=> $paypal_link
    ),
  	'success'	=> array(
      '#type'		        => 'textfield',
      '#title'					=> t("Success URL"),
      '#default_value'	=> (isset($config['success']))? $config['success'] : '',
      '#description'		=> t("The page the user will be returned to after a successful sale.  If you leave this blank, Paypal API will supply a suitable generic page.")
    ),
  	'cancel'	=> array(
      '#type'		        => 'textfield',
      '#title'					=> t("Cancel URL"),
      '#default_value'	=> (isset($config['cancel']))? $config['cancel'] : '',
      '#description'		=> t("The page the user will be returned to after a cancelled sale.  If you leave this blank, Paypal API will supply a suitable generic page.")
    ),
    'other_variables' => array(
      '#markup' => t("Paypal offers a variety of further variables that may be sent with the transaction.  If you want to set those variables up, implement hook_paypal_api_prepay() and add them at the time of the transaction.  See pyapal_api.api.php for more documentation.")
    )
  );

  $form['submit'] = array(
    '#type'		=> 'submit',
    '#value'	=> t("Save these settings")
  );

  $form['help'] = array(
      '#markup'	=> "
        <p>
          " . t("You can deactivate this content type for Paypal payments.  !link to deactivate this content now.", array('!link' => l(t("Edit this content type"), 'admin/structure/types/manage/' . $bundle))) . "
        </p>
        <p>
          " . l(t("Manage global Paypal API settings"), 'admin/config/system/paypal_api') . "
        </p>
      "
  );

  return $form;
}

/**
 *
 * Validate the type settings form.
 *
 * @param $form_state
 */
function paypal_api_type_settings_validate($form, &$form_state) {
  // op checkboxes
  if (!$form_state['values']['view'] && !$form_state['values']['create'] && !$form_state['values']['instance'] && !$form_state['values']['deferred_create']) {
    form_set_error('view', t("You must select at least one node operation to require payment for."));
  }

  if (!is_numeric($form_state['values']['quantity'])) {
    form_set_error('quantity', t("This field must be a number."));
  }

  // paypal buttons
  if ($form_state['values']['method'] == 'paypal') {
    if (($form_state['values']['view'] || $form_state['values']['create']) && !$form_state['values']['button']) {
      form_set_error('button', t("You must insert button code from Paypal if you want to require payment to view or create content of this type."));
    }
  }

  // api buttons
  elseif ($form_state['values']['method'] == 'api') {
    if ($form_state['values']['cost'] && !is_numeric($form_state['values']['cost'])) {
      form_set_error('cost', t("Default cost must be a number."));
    }
  }

  // no button selected
  else {
    form_set_error('method', t("You must select a button generation method."));
  }
}

/**
 *
 * Save the type settings form.
 *
 * @param $form_state
 */
function paypal_api_type_settings_submit($form, &$form_state) {
  $settings = variable_get('paypal_api_settings', array());

  foreach (array('view', 'instance', 'create', 'deferred_create', 'method', 'button', 'cost', 'currency', 'item_name', 'success', 'cancel', 'quantity') as $field) {
    $config[$field] = $form_state['values'][$field];
  }

  $settings['types'][$form_state['values']['type']] = $config;
  variable_set('paypal_api_settings', $settings);

  // if this previously was an instance type, we need to remove the existing
  // records in the instance table, or node access will break
  if (!$config['instance']) {
    paypal_api_remove_instances($form_state['values']['type']);
  }

  drupal_set_message(t("Your settings have been updated."), 'status');

  $vars = array('@bundle' => $form_state['values']['type'], '!permissions_link' => l(t('user permissions'), 'admin/people/permissions'));
  if ($config['view']) {
    drupal_set_message(t("Users will be able to view ANY '@bundle' node content after payment.  Check !permissions_link, as basic permissions take precedence over these settings.", $vars), 'warning');
  }
  if ($config['instance']) {
    drupal_set_message(t("Users will be able to view A SINGLE '@bundle' node after payment.  You may indicated which node by editing the specific node you wish to grant access to.  Users will not be able to view other nodes of the same content type.  Check !permissions_link, as basic permissions take precedence over these settings.", $vars), 'warning');
  }
  if ($config['create']) {
    drupal_set_message(t("Users will be able to CREATE '@bundle' nodes after payment.  Check !permissions_link, as basic permissions take precedence over these settings.", $vars), 'warning');
  }
  if ($config['deferred_create']) {
    drupal_set_message(t("Users will be able to CREATE '@bundle' nodes at will, but the nodes will not be published until after payment.  Check !permissions_link, as basic permissions take precedence over these settings.", $vars), 'warning');
  }
}
