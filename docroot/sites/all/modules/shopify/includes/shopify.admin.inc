<?php
/**
 * @file
 * Contains functions and forms for admin pages.
 */

/**
 * Theme download admin page.
 */
function shopify_admin_theme_download($form, $form_state) {
  $form = array();
  $form['download'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shopify Theme'),
    '#description' => t('Download this starter Shopify theme then upload it manually to your <a target="_blank" href="@shopify_store_link">Shopify store</a>, or use the "Upload and Publish Automatically" feature. <span style="color: red">Automatic upload requires this website be publicly accessible from the internet.</span><br /><br />This theme will disable most Shopify store features except for the shopping cart and customer login area, and will redirect the user to your store on your Drupal site if they attempt to access areas covered by this module like products, tags, or collections.<br /><br />We <strong>highly recommend</strong> using this theme generator as a starting point for your Shopify theme. Once uploaded you may use the Shopify theme GUI to match your site\'s colors, fonts, etc.', array(
      '@shopify_store_link' => url('http://' . shopify_store_info()->myshopify_domain . '/admin/themes'),
    )),
  );
  $form['download']['actions'] = array(
    '#type' => 'actions',
    'download' => array(
      '#type' => 'submit',
      '#value' => t('Download Only'),
      '#name' => 'download',
    ),
    'upload' => array(
      '#type' => 'submit',
      '#value' => t('Upload and Publish Automatically'),
      '#description' => t('Will be automatically uploaded to your Shopify account and set as the active theme.'),
      '#name' => 'upload',
    ),
  );

  $protocol = stripos($_SERVER['SERVER_PROTOCOL'], 'https') === TRUE ? 'https://' : 'http://';
  $fqdn = $protocol . "$_SERVER[HTTP_HOST]";
  $form['download']['hostname'] = array(
    '#type' => 'textfield',
    '#title' => t('Hostname'),
    '#default_value' => $fqdn,
    '#size' => 60,
    '#required' => TRUE,
    '#description' => t('What hostname should the Shopify theme link back to?'),
  );

  return $form;
}

/**
 * Validation handler for theme download form.
 */
function shopify_admin_theme_download_validate($form, &$form_state) {
  $hostname = &$form_state['values']['hostname'];
  if (substr($hostname, -1) != '/') {
    $hostname .= '/';
  }
}

/**
 * Submit callback for download the Shopify theme folder.
 */
function shopify_admin_theme_download_submit($form, $form_state) {
  $fqdn = $form_state['values']['hostname'];
  // Copy the default theme to a temp folder.
  try {
    shopify_recurse_copy(drupal_get_path('module', 'shopify') . '/default_shopify_theme', file_directory_temp() . '/default_shopify_theme');
  } catch (Exception $e) {
    drupal_set_message(t('Could not copy default_shopify_theme to the temp directory: @error', array('@error' => $e->getMessage())), 'error');
  }

  // Modify the {{ replace }} contents within theme files.
  try {
    shopify_find_and_replace(file_directory_temp() . '/default_shopify_theme/*', '{{ drupal.site.url }}', url($fqdn, array('absolute' => TRUE)));
  } catch (Exception $e) {
    drupal_set_message(t('Could not find and replace placeholder text: @error', array('@error' => $e->getMessage())), 'error');
  }

  // Modify the {{ replace }} contents within theme files.
  try {
    shopify_find_and_replace(file_directory_temp() . '/default_shopify_theme/*', '{{ drupal.store.url }}', url($fqdn . shopify_store_path(), array('absolute' => TRUE)));
  } catch (Exception $e) {
    drupal_set_message(t('Could not find and replace placeholder text: @error', array('@error' => $e->getMessage())), 'error');
  }

  // Zip the theme folder.
  try {
    shopify_zip_folder(file_directory_temp() . '/default_shopify_theme');
  } catch (Exception $e) {
    drupal_set_message(t('Could not ZIP the default_shopify_theme folder: @error', array('@error' => $e->getMessage())), 'error');
  }

  if ($form_state['triggering_element']["#name"] == 'download') {
    // Download the file to the user's browser.
    try {
      shopify_theme_download('default_shopify_theme.zip');
    } catch (Exception $e) {
      drupal_set_message(t('Could not download the ZIP folder: @error', array('@error' => $e->getMessage())), 'error');
    }
  }
  elseif ($form_state['triggering_element']['#name'] == 'upload') {
    // Try to upload this zip file for the user.
    try {
      // Create a secure URL.
      $sig = hash_hmac('sha256', REQUEST_TIME . 'default_shopify_theme.zip', variable_get('shopify_api_secret'));
      shopify_api_call('POST', 'themes', array(
        'theme' => array(
          'name' => 'Drupal Shopify Theme',
          'src' => url('shopify/theme/download/' . REQUEST_TIME . '/' . $sig . '/default_shopify_theme.zip', array('absolute' => TRUE)),
          'role' => 'main',
        ),
      ));
      drupal_set_message(t('Drupal Shopify Theme was uploaded to your store. !link.', array(
        '!link' => l('View now', 'https://' . shopify_store_info()->myshopify_domain . '/admin/themes', array('attributes' => array('target' => '_blank'))),
      )));
    } catch (Exception $e) {
      drupal_set_message(t('Could not upload the ZIP folder: @error', array('@error' => $e->getMessage())), 'error');
    }
  }
}

/**
 * ZIP a folder.
 *
 * @param string $path
 *   Path to the folder.
 *
 * @link http://stackoverflow.com/questions/4914750/how-to-zip-a-whole-folder-using-php @endlink
 */
function shopify_zip_folder($path) {
  // Get real path for our folder
  $root_path = realpath($path);

  // Initialize archive object
  $zip = new ZipArchive();
  $zip->open($path . '.zip', ZipArchive::CREATE | ZipArchive::OVERWRITE);

  // Create recursive directory iterator
  $files = new RecursiveIteratorIterator(
    new RecursiveDirectoryIterator($root_path),
    RecursiveIteratorIterator::LEAVES_ONLY
  );

  foreach ($files as $name => $file) {
    // Skip directories (they would be added automatically)
    if (!$file->isDir()) {
      // Get real and relative path for current file
      $file_path = $file->getRealPath();
      $relative_path = substr($file_path, strlen($root_path) + 1);

      // Add current file to archive
      $zip->addFile($file_path, $relative_path);
    }
  }

  // Zip archive will be created only after closing object
  $zip->close();
}

/**
 * Find and replace text in a folder recursively.
 *
 * @param string $dir
 *   Directory to search in.
 * @param string $find_text
 *   Text to find.
 * @param string $replace_text
 *   Text to replace the found text.
 */
function shopify_find_and_replace($dir, $find_text, $replace_text) {
  foreach (glob($dir) as $path_to_file) {
    if (strpos($path_to_file, '.') === FALSE) {
      // This is a directory.
      shopify_find_and_replace($path_to_file . '/*', $find_text, $replace_text);
    }
    else {
      $file_contents = file_get_contents($path_to_file);
      $file_contents = str_replace($find_text, $replace_text, $file_contents);
      file_put_contents($path_to_file, $file_contents);
    }
  }
}

/**
 * Copy an entire directory and contents.
 *
 * @param string $src
 *   Source directory.
 * @param string $dst
 *   Destination directory.
 *
 * @link http://stackoverflow.com/questions/2050859/copy-entire-contents-of-a-directory-to-another-using-php @endlink
 */
function shopify_recurse_copy($src, $dst) {
  $dir = opendir($src);
  @mkdir($dst);
  while (FALSE !== ($file = readdir($dir))) {
    if (($file != '.') && ($file != '..')) {
      if (is_dir($src . '/' . $file)) {
        shopify_recurse_copy($src . '/' . $file, $dst . '/' . $file);
      }
      else {
        copy($src . '/' . $file, $dst . '/' . $file);
      }
    }
  }
  closedir($dir);
}

/**
 * Admin page callback.
 */
function shopify_admin_page() {
  try {
    $info = shopify_api_get_shop_info(TRUE);
  } catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
    return t('<ul><li>Do you have a <a target="_blank" href="@signup">Shopify account</a>?</li><li>If you have an account you\'ll need to create your API key if you haven\'t already. See <a target="_blank" href="@api_key">https://docs.shopify.com/support/configuration/apps/where-do-i-find-my-api-key</a> for more information.</li><li>Have you checked your <a href="@api_config">API settings to make sure they match?</a></li><li><a href="@status_page">Can your server make outgoing HTTP requests?</li></ul>', array(
      '@signup' => url('https://www.shopify.com/?ref=bonify'),
      '@api_key' => url('https://docs.shopify.com/support/configuration/apps/where-do-i-find-my-api-key'),
      '@api_config' => url('admin/config/services/shopify'),
      '@status_page' => url('/admin/reports/status'),
    ));
  }

  // Get count of products in the site.
  $product_count = db_select('shopify_products', 'pr')
    ->fields('pr')
    ->execute()->rowCount();

  if (!$product_count) {
    drupal_set_message(t('Did you !link to your site yet?', array(
      '!link' => l(t('sync products'), 'admin/shopify/sync'),
    )), 'warning');
  }

  // Get count of collections in the site.
  $collection_count = db_select('taxonomy_term_data', 'ttd')
    ->fields('ttd')
    ->condition('vid', shopify_collection_vocabulary()->vid)
    ->execute()->rowCount();

  if (!$collection_count) {
    drupal_set_message(t('Did you !link to your site yet?', array(
      '!link' => l(t('sync collections'), 'admin/shopify/sync'),
    )), 'warning');
  }

  $store_meta = array(
    'My Store Admin' => l($info->myshopify_domain, 'https://' . $info->myshopify_domain . '/admin'),
    'Owned By' => $info->shop_owner . ' &lt;<a href="mailto:' . $info->email . '">' . $info->email . '</a>&gt;',
    'Address' => $info->address1,
    'City' => $info->city,
    'State/Province' => $info->province,
    'Zip' => $info->zip,
    '# of products' => $product_count,
    '# of collections' => $collection_count,
  );

  $return = '';
  foreach ($store_meta as $label => $meta) {
    $return .= '<strong>' . $label . '</strong>: ' . $meta . '<br />';
  }
  return $return;
}

/**
 * Shopify products sync page form.
 */
function shopify_admin_sync_form($form, &$form_state) {

  try {
    shopify_api_get_shop_info(TRUE);
  } catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
    return array();
  }

  $last_updated = variable_get('shopify_last_product_sync');
  if ($last_updated == '1970-01-01 00:00' || !$last_updated) {
    $last_updated = 'never';
  }

  $info = 'Last sync !last_updated. Only products updated since the last sync will ';
  $info .= 'be pulled and synced. Products updated via webhooks will be updated anyway. ';
  $info .= 'Custom fields you\'ve added yourself will not be overridden on update.';

  $form['products'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sync Products'),
    '#description' => t($info, array('!last_updated' => $last_updated)),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  $form['products']['num_per_batch'] = array(
    '#type' => 'select',
    '#title' => 'Choose how many products to sync per batch operation (not per batch).',
    '#options' => array(
      '1' => t('One at a time'),
      '50' => t('50 at a time'),
      '250' => t('250 (Max API limit)'),
    ),
    '#default_value' => 250,
  );

  $form['products']['delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete all products then re-import fresh.') . '<br /><strong>' . t('CAUTION: Product entities will be completely deleted then re-imported. Custom fields will be erased, comments deleted, etc.') . '</strong>',
  );

  $form['products']['update_anyway'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update all products regardless of last sync time. Product entities will be updated, not deleted.'),
  );

  $form['products']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Sync Products',
    '#submit' => array('shopify_admin_product_sync_form_submit'),
  );

  $last_updated = variable_get('shopify_last_collection_sync');
  if ($last_updated == '1970-01-01 00:00' || !$last_updated) {
    $last_updated = 'never';
  }
  $form['collections'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sync Collections'),
    '#description' => t('Last sync !last_updated.', array('!last_updated' => $last_updated)),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
  $form['collections']['delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete all collections then re-import fresh.') . '<br /><strong>' . t('CAUTION: Collection terms will be completely deleted then re-imported. Custom fields will be erased, comments deleted, etc.') . '</strong>',
  );
  $form['collections']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Sync Collections',
    '#submit' => array('shopify_admin_collection_sync_form_submit'),
  );

  $form['cron'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cron'),
    '#description' => t('Settings for automatically syncing products/collections on cron run.<br /><strong>Only newly updated products/collections will be synced.</strong><br /><br />'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Cron settings.
  $form['cron']['sync_products'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync Products on cron run.'),
    '#default_value' => variable_get('shopify_cron_sync_products', FALSE),
  );
  $form['cron']['sync_collections'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync Collections on cron run.'),
    '#default_value' => variable_get('shopify_cron_sync_collections', FALSE),
  );
  $form['cron']['sync_time'] = array(
    '#type' => 'textfield',
    '#title' => t('How often to sync'),
    '#description' => t('Enter the number of seconds to wait to sync between cron runs.<br />To sync once per day, enter "86400". To sync once per hour, enter "3600".<br />Leave empty or "0" to sync on every cron run.'),
    '#default_value' => variable_get('shopify_cron_sync_time', 0),
  );
  $form['cron']['save_cron'] = array(
    '#type' => 'submit',
    '#value' => t('Save cron settings'),
    '#submit' => array('shopify_admin_cron_sync_form_submit'),
  );

  if (shopify_get_orphaned_tags()) {
    $form['delete_orphaned_tags'] = array(
      '#markup' => l(t('Delete orphaned tags'), 'admin/shopify/sync/delete-orphaned-tags'),
    );
  }

  return $form;
}

/**
 * Deleting orphaned tags confirmation form.
 */
function shopify_admin_delete_orphaned_tags_form($form, $form_state) {
  $terms = shopify_get_orphaned_tags();
  $list = array();
  foreach ($terms as $term) {
    $list[] = $term->name;
  }
  $form['deleting_terms'] = array(
    '#theme' => 'item_list',
    '#items' => $list,
  );
  return confirm_form($form, t('Delete orphaned product tag taxonomy terms?'), 'admin/shopify/sync', t('<strong>WARNING:</strong> You are about to delete @count.<br/>', array(
    '@count' => format_plural(count($terms), '@count term', '@count terms'),
  )));
}

/**
 * Submit handler for deleting orphaned tags.
 */
function shopify_admin_delete_orphaned_tags_form_submit($form, &$form_state) {
  // At this point we can safely delete the terms.
  $terms = shopify_get_orphaned_tags();
  foreach ($terms as $term) {
    taxonomy_term_delete($term->tid);
    drupal_set_message(t('Deleted term "%name".', array('%name' => $term->name)));
  }
  $form_state['redirect'] = 'admin/shopify/sync';
}

/**
 * Submit handler for cron settings save.
 */
function shopify_admin_cron_sync_form_submit($form, $form_state) {
  $v = $form_state['values'];
  variable_set('shopify_cron_sync_products', $v['cron']['sync_products']);
  variable_set('shopify_cron_sync_collections', $v['cron']['sync_collections']);
  variable_set('shopify_cron_sync_time', $v['cron']['sync_time']);
}

/**
 * Submit handler for collection syncing.
 */
function shopify_admin_collection_sync_form_submit($form, &$form_state) {
  if ($form_state['values']['collections']['delete']) {
    shopify_delete_all_collections();
  }
  $batch = shopify_sync_collections_batch(array('delete_collections' => $form_state['values']['collections']['delete']));
  batch_set($batch);
}

/**
 * Shopify collections sync batch callback.
 */
function shopify_sync_collections_batch($params) {
  // Get all smart/custom collection data.
  $collections = shopify_api_get_collections(array('limit' => 250));
  $operations = array();
  foreach ($collections as $count => $c) {
    $operations[] = array(
      'shopify_sync_collections_batch_op',
      array(
        $c,
        t('(Processing page @operation)', array('@operation' => $count + 1)),
      ),
    );
  }
  if (!$params['delete_collections']) {
    // Last operation to delete orphaned collections (deleted in Shopify).
    $operations[] = array(
      'shopify_sync_collections_deleted_batch_op',
      array(),
    );
  }
  $batch = array(
    'operations' => $operations,
    'finished' => 'shopify_sync_collections_batch_finished',
    'file' => drupal_get_path('module', 'shopify') . '/includes/shopify.admin.inc',
  );
  return $batch;
}

/**
 * Batch operation for syncing Shopify collections.
 *
 * @param array $collection
 *   Collection data.
 * @param string $operation_details
 *   Operation details.
 * @param array $context
 *   Batch context.
 */
function shopify_sync_collections_batch_op($collection, $operation_details, &$context) {
  $term = shopify_collection_load($collection['id']);

  if (!$term) {
    // Need to create a new collection.
    shopify_collection_create($collection, TRUE);
  }
  else {
    shopify_collection_update($collection, TRUE);
  }

  $context['results'][] = $collection;
  $context['message'] = t('Syncing collection @title...', array('@title' => $collection['title']));
}

/**
 * Batch finished callback for collections batch.
 */
function shopify_sync_collections_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('Processed @count collections.', array('@count' => count($results))));
    $datetime = new DateTime();
    $last_updated = $datetime->format('Y-m-d H:i');
    variable_set('shopify_last_collection_sync', $last_updated);
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    drupal_set_message(
      t('An error occurred while processing @operation with arguments : @args',
        array(
          '@operation' => $error_operation[0],
          '@args' => print_r($error_operation[0], TRUE),
        )
      )
    );
  }
}

/**
 * Shopify product sync form submit handler.
 *
 * @see shopify_admin_product_sync_form()
 */
function shopify_admin_product_sync_form_submit($form, &$form_state) {
  // @todo Probably should have a batch just for this.
  if ($form_state['values']['products']['delete']) {
    shopify_delete_all_products();
    shopify_products_reset_sync_datestamp();
  }
  if ($form_state['values']['products']['update_anyway']) {
    shopify_products_reset_sync_datestamp();
  }
  $batch = shopify_sync_products_batch(array(
    'delete_products' => $form_state['values']['products']['delete'],
    'num_per_batch' => $form_state['values']['products']['num_per_batch'],
  ));
  batch_set($batch);
}


/**
 * Shopify products sync batch callback.
 */
function shopify_sync_products_batch($params) {
  // Get count of products
  $num_products = shopify_api_get_product_count(
    array(
      'updated_at_min' => variable_get('shopify_last_product_sync')
    )
  );
  $num_operations = ceil($num_products / $params['num_per_batch']);
  $operations = array();
  for ($page = 1; $page <= $num_operations; $page++) {
    $operations[] = array(
      'shopify_sync_products_batch_op',
      array(
        $page,
        $params['num_per_batch'],
        t('(Processing page @operation)', array('@operation' => $page)),
      ),
    );
  }

  // Only run the deleted batch op if we are NOT deleting all products first.
  if (!$params['delete_products']) {
    // Last operation to delete orphaned products (deleted in Shopify).
    $operations[] = array(
      'shopify_sync_products_deleted_batch_op',
      array(),
    );
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'shopify_sync_products_batch_finished',
    'file' => drupal_get_path('module', 'shopify') . '/includes/shopify.admin.inc',
  );
  return $batch;
}

/**
 * Delete collections in the system that were deleted on Shopify.
 */
function shopify_sync_collections_deleted_batch_op(&$context) {
  // Tell the user that we deleted some collections.
  if ($deleted = shopify_sync_deleted_collections(array('limit' => 250)) > 0) {
    drupal_set_message(t('Deleted @count from the store.', array(
      '@count' => format_plural($deleted, '@count collection', '@count collections'),
    )));
    $context['message'] = t('Deleting orphaned collections...');
  }
}

/**
 * Delete products in the system that were deleted on Shopify.
 */
function shopify_sync_products_deleted_batch_op(&$context) {
  // Tell the user that we deleted some products/variants.
  if ($deleted = shopify_sync_deleted_products(array('limit' => 250)) > 0) {
    drupal_set_message(t('Deleted @count from the store.', array(
      '@count' => format_plural($deleted, '@count product/variant', '@count products/variants'),
    )));
    $context['message'] = t('Deleting orphaned products/variants...');
  }
}

/**
 * Shopify products sync batch operation handler.
 */
function shopify_sync_products_batch_op($page, $num_per_batch, $operation_details, &$context) {
  $products = shopify_api_get_products(
    array(
      'page' => $page,
      'limit' => $num_per_batch,
      'updated_at_min' => variable_get('shopify_last_product_sync')
    )
  );
  foreach ($products as $product) {
    $shopify_product = shopify_product_update($product['id'], 0, $product);
    $shopify_product->save();

    foreach ($product['variants'] as $v) {
      $variant = shopify_product_update($product['id'], $v['id'], $v);
      $variant->save();
      $context['results'][] = $variant->product_id . ' : ' . check_plain($variant->title);
    }
    $context['results'][] = $shopify_product->product_id . ' : ' . check_plain($shopify_product->title);
  }

  $context['message'] = t('Syncing product "@title" and its variants', array('@title' => $shopify_product->title)) . ' ' . $operation_details;
}

/**
 * Shopify product sync batch operation finished callback.
 *
 * @see shopify_sync_products_batch_op()
 */
function shopify_sync_products_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('Processed @count products and variants.', array('@count' => count($results))));
    $datetime = new DateTime();
    $last_updated = $datetime->format('Y-m-d H:i');
    variable_set('shopify_last_product_sync', $last_updated);
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    drupal_set_message(
      t('An error occurred while processing @operation with arguments : @args',
        array(
          '@operation' => $error_operation[0],
          '@args' => print_r($error_operation[0], TRUE),
        )
      )
    );
  }
}

/**
 * Shopify webhooks admin form.
 */
function shopify_webhooks_admin($form, &$form_state) {
  try {
    $webhooks = shopify_api_get_webhooks();
  } catch (Exception $e) {
    $message = $e->getMessage();
    drupal_set_message($message, 'error');
    return '';
  }
  $form['#tree'] = TRUE;

  $form['help'] = array(
    '#markup' => t('<p>It is <strong>highly recommended</strong> that you at least have webhooks registered for product and collection events to keep your store automatically in sync with Shopify. Make sure you\'ve registered all development, staging and production environment URLs.</p>'),
  );

  $message = '<p>Pro Tip: If you\'re developing locally and need to test webhooks ';
  $message .= 'download and install <a href="https://ngrok.com">ngrok</a> for a tunnel to your localhost. ';
  $message .= 'The service is free. Here is a command-line example: <br /><code>';
  $message .= 'ngrok http -host-header=YOUR_LOCALHOST_NAME_HERE.COM 80</code>. ';
  $message .= 'Place the "Forwarding" address into the hostname field above and ';
  $message .= 'register the hooks you want to test. The forwarding address ';
  $message .= 'address will look something like <code>http://0e1ff1cb.ngrok.io</code></p>';

  $form['tips'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tips'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['tips']['UI'] = array(
    '#markup' => t('<p>Webhooks created via the Shopify website will not appear here AND events will not trigger these hooks.</p>'),
  );

  $form['tips']['standard_hooks'] = array(
    '#markup' => t('<p>Be sure to add the product/collection webhooks to automatically keep your products/collections in sync with Shopify. Other hooks do nothing unless you write code implementing the hook yourself.</p>'),
  );

  $form['tips']['ngrok'] = array(
    '#markup' => t($message),
  );

  $form['registered'] = array(
    '#type' => 'fieldset',
    '#title' => t('Webhooks Registered'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if (empty($webhooks)) {
    $form['registered']['none'] = array(
      '#markup' => t('<p>No webhooks registed on Shopify for this App.</p><p>It is highly recommended that you at least have webhooks registered for product events to keep your store automatically in sync with Shopify.</p><p>Hooks created via the Shopify website will not be displayed here but will still work with the hook system. Caution: Internal paths are different for each hook.</p>'),
    );
  }

  foreach ($webhooks as $hook) {
    $matches = array();
    preg_match('/:\/\/(.+)\/shopify\/webhook/', $hook['address'], $matches);
    if (!isset($form['registered'][$hook['address']])) {
      $form['registered'][$hook['address']] = array(
        '#type' => 'fieldset',
        '#title' => check_plain($matches[1]),
        '#collapsible' => TRUE,
      );
    }
    $form['registered'][$hook['address']][$hook['id']] = array(
      '#type' => 'checkbox',
      '#title' => $hook['topic'],
      '#description' => t('React to this event using: @hook', array('@hook' => 'HOOK_shopify_webook_' . str_replace('/', '_', $hook['topic']))),
    );
  }

  $form['registered']['remove_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Remove'),
    '#submit' => array('shopify_remove_webhooks_submit'),
  );

  $form['register'] = array(
    '#type' => 'fieldset',
    '#title' => t('Register New Webhooks'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $possible_hooks = array(
    'app' => array(
      'uninstalled' => t('App Uninstalled'),
    ),
    'carts' => array(
      'create' => t('Cart Created'),
      'update' => t('Cart Updated'),
    ),
    'checkouts' => array(
      'create' => t('Checkout Created'),
      'update' => t('Checkout Updated'),
      'delete' => t('Checkout Deleted'),
    ),
    'collections' => array(
      'create' => t('Collection Created - Highly Recommended'),
      'update' => t('Collection Updated - Highly Recommended. Bug on Shopify side, see <a target="_blank" href="@issue">Issue</a>', array('@issue' => url('https://www.drupal.org/node/2481105'))),
      'delete' => t('Collection Deleted - Highly Recommended'),
    ),
    'customers' => array(
      'create' => t('Customer Created'),
      'enable' => t('Customer Enabled'),
      'disable' => t('Customer Disabled'),
      'update' => t('Customer Updated'),
      'delete' => t('Customer Deleted'),
    ),
    'customer_groups' => array(
      'create' => t('Customer Group Created'),
      'update' => t('Customer Group Updated'),
      'delete' => t('Customer Group Deleted'),
    ),
    'fulfillments' => array(
      'create' => t('Fulfillment Created'),
      'update' => t('Fullfillment Updated'),
    ),
    'orders' => array(
      'create' => t('Order Created'),
      'delete' => t('Order Deleted'),
      'updated' => t('Order Updated'),
      'paid' => t('Order Paid'),
      'cancelled' => t('Order Cancelled'),
      'fulfilled' => t('Order Fulfilled'),
      'partially_fulfilled' => t('Order Partially Fulfilled'),
    ),
    'order_transactions' => array(
      'create' => t('Order Transaction Created'),
    ),
    'products' => array(
      'create' => t('Product Created - Highly Recommended'),
      'update' => t('Product Updated - Highly Recommended'),
      'delete' => t('Product Deleted - Highly Recommended'),
    ),
    'refunds' => array(
      'create' => t('Refund Created'),
    ),
    'shop' => array(
      'update' => t('Shop Updated'),
    ),
  );

  foreach ($possible_hooks as $group_name => $group_options) {

    $form['register'][$group_name] = array(
      '#type' => 'fieldset',
      '#title' => t(str_replace('_', ' ', ucwords($group_name))),
      '#collapsible' => TRUE,
      '#collapsed' => ($group_name == 'products' || $group_name == 'collections') ? FALSE : TRUE,
    );

    foreach ($group_options as $topic => $description) {
      $form['register'][$group_name][$topic][$description] = array(
        '#type' => 'checkbox',
        '#title' => $description,
      );
    }
  }

  $protocol = stripos($_SERVER['SERVER_PROTOCOL'], 'https') === TRUE ? 'https://' : 'http://';
  $current = $protocol . "$_SERVER[HTTP_HOST]";

  $form['register']['hostname'] = array(
    '#type' => 'textfield',
    '#title' => t('Hostname'),
    '#default_value' => variable_get('shopify_last_registered_host', $current),
    '#size' => 60,
    '#required' => TRUE,
    '#description' => t('Do not include a trailing slash.'),
  );

  $form['debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug webhooks in watchdog'),
    '#default_value' => variable_get('shopify_webhooks_debug', FALSE),
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('shopify_register_webhooks_submit'),
    '#validate' => array('shopify_register_webhooks_validate'),
  );

  return $form;
}

/**
 * Shopify webhooks admin form remove submit handler.
 */
function shopify_remove_webhooks_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Remove')) {
    unset($form_state['values']['registered']['remove_submit']);
    foreach ((array) $form_state['values']['registered'] as $host => $values) {
      foreach ((array) $values as $id => $remove) {
        if ($remove) {
          shopify_api_delete_webhook($id);
        }
      }
    }
  }
}

/**
 * Shopify webhooks admin form validation handler.
 */
function shopify_register_webhooks_validate($form, &$form_state) {
  $hostname = &$form_state['values']['register']['hostname'];
  $hostname = rtrim($hostname, '/');
}

/**
 * Shopify webhooks register submit handler.
 */
function shopify_register_webhooks_submit($form, &$form_state) {
  $values = $form_state['values'];

  variable_set('shopify_webhooks_debug', $values['debug']);
  variable_set('shopify_last_registered_host', $values['register']['hostname']);

  if ($form_state['values']['op'] == t('Save')) {

    unset($values['save']);
    $hostname = $values['register']['hostname'];
    unset($values['register']['hostname']);

    foreach ($values['register'] as $group => $topics) {
      foreach ($topics as $topic => $selection) {
        if (reset($selection) == 1) {
          $hook = array(
            'webhook' => array(
              'topic' => "$group/$topic",
              'address' => $hostname . '/shopify/webhook',
              'format' => "json"
            ),
          );
          try {
            shopify_api_create_webhook($hook);
          } catch (ShopifyApiException $e) {
            $response = $e->getResponse();
            if (isset($response['errors']['address'])) {
              $message = $hostname . ' address ' . $response['errors']['address'][0];
            }
            else {
              $message = $response['errors'];
            }
            drupal_set_message(t('Shopify says there was a problem with @topic: @errors', array(
              '@errors' => $message,
              '@topic' => "$group/$topic"
            )), 'warning');
          }
        }
      }
    }
  }
}
