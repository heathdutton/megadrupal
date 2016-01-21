<?php

/**
 * Uploads an HTML page to Smartling by a given URL and username
 * @param $username
 * @param $url
 * @param bool|FALSE $force_show
 * @return mixed|string|void
 * @throws \Exception
 */
function smartling_translation_context_upload_context($username, $url, $force_show = FALSE) {
  $url .= (strpos($url, '?') === FALSE) ? '?' : '&';
  $url .= 'smartling_context=1';

  module_load_include('inc', 'smartling_translation_context', 'includes/context_authorize');
  module_load_include('inc', 'smartling_translation_context', 'includes/url_to_absolute');
  module_load_include('inc', 'smartling_translation_context', 'includes/html_save_complete');

  $auth = smartling_translation_context_authorize($username);

  $page = new HtmlSaveComplete();
  $html = $page->getCompletePage($url, $auth['response']['data']['cookie']);
  $html = str_replace('<html ', '<html class="sl-override-context" ', $html);

  if ($force_show) {
    return $html;
  }

  //returns updated strings count
  $updatedStringsCount = drupal_container()
    ->get('smartling.api_wrapper')
    ->uploadContext(array('url' => $url, 'html' => $html));


    if ($updatedStringsCount < 0) {
      smartling_log_get_handler()->error(t('There was an error during context upload for entity with url: "@url" and user: "@user". Please <a href="@url_logs">see logs</a> for details.', array(
        '@url_logs' => url('admin/reports/dblog'),
        '@url' => $url,
        '@user' => $username,
      )));
    }
    else {
      smartling_log_get_handler()->info(t('Smartling updated context for @updatedStringsCount strings  for entity with url: "@url" and user: "@user".', array(
        '@updatedStringsCount' => $updatedStringsCount,
        '@url' => $url,
        '@user' => $username,
      )));
    }
  return $updatedStringsCount;
}

/**
 * Wrapper around smartling_translation_context_upload_context().
 * Uploads context for a given entity.
 * @param $entity_type
 * @param $entity
 * @return mixed|string|void
 */
function smartling_translation_context_upload_entity($entity_type, $entity) {
  global $user;

  if (isset($entity->smartling_context_url)) {
    $url = $entity->smartling_context_url;
  }
  else {
    $url = entity_uri($entity_type, $entity);
    $url = url($url['path'], array('absolute' => TRUE));
  }

  $name = isset($user->name) ? $user->name : user_load(1)->name;
  $username = variable_get('smartling_translation_context_username', $name);

  return smartling_translation_context_upload_context($username, $url);
}

/**
 * Callback for cron handling.
 */
function smartling_translation_context_on_cron_run_upload() {
  $query = db_select('smartling_context', 'sc');
  $query->fields('sc', array('eid', 'entity_type'));
  $query->condition('sc.when_to_upload', REQUEST_TIME, '<');
  $query->condition('sc.status', SMARTLING_CONTEXT_STATUS_TO_UPLOAD, '=');
  $result = $query->execute()->fetchAll();

  foreach($result as $v) {
    $entity = entity_load_single($v->entity_type, $v->eid);
    smartling_translation_context_upload_entity($v->entity_type, $entity);

    db_merge('smartling_context')
      ->key(array('eid' => $v->eid, 'entity_type' => $v->entity_type))
      ->fields(array(
        'last_uploaded' => REQUEST_TIME,
        'status' => SMARTLING_CONTEXT_STATUS_UPLOADED,
      ))
      ->execute();
  }
}