<?php
/**
 * @file
 * Restore operation: redirect class.
 */

/**
 * The redirect restore operation class.
 */
class RestoreRedirectOperationRedirect extends RestoreOperation {

  /**
   * Overriden RestoreOperation::item().
   */
  protected function item($key) {
    return db_select('redirect', 'rd')
      ->fields('rd', array('source', 'redirect', 'language', 'status_code'))
      ->condition('source', $key)
      ->execute()
      ->fetchAssoc();
  }

  /**
   * Overriden RestoreOperation::itemLabel().
   */
  protected function itemLabel($key, $item) {
    return "{$item['redirect']} => <em>{$item['source']}</em>";
  }

  /**
   * Overriden RestoreOperation::itemStatus().
   */
  protected function itemStatus($key, $item) {
    $redirect = redirect_load_by_source($key);
    if ($redirect) {
      return (
        $item['source'] == $redirect->source &&
        $item['redirect'] == $redirect->redirect &&
        (isset($item['status_code']) ? $item['status_code'] : 0) == $redirect->status_code &&
        (isset($item['language']) ? $item['language'] : LANGUAGE_NONE) == $redirect->language
      ) ? RestoreScript::STATUS_ACTIVE : RestoreScript::STATUS_OVERRIDEN;
    }
    else {
      return RestoreScript::STATUS_MISSING;
    }
  }

  /**
   * Overriden RestoreOperation::itemRestore().
   */
  protected function itemRestore($key, $item) {
    $redirect = redirect_load_by_source($key);
    if (isset($redirect->rid)) {
      continue;
    }

    // Create the redirect object.
    $redirect = new stdClass();

    redirect_object_prepare($redirect);
    $redirect->source = $key;
    $redirect->redirect = $item['redirect'];
    $redirect->status_code = isset($item['status_code']) ? $item['status_code'] : 0;
    $redirect->language = isset($item['language']) ? $item['language'] : LANGUAGE_NONE;

    // Check if the redirect exists before saving.
    $hash = redirect_hash($redirect);
    if (!redirect_load_by_hash($hash)) {
      redirect_save($redirect);
    }
  }

  /**
   * Overriden RestoreOperation::available().
   */
  public function available($filter = FALSE, $show_selected = FALSE) {
    $redirects = array();
    $dbq = db_select('redirect', 'rd')
      ->fields('rd', array('source', 'redirect', 'language', 'status_code'));

    if ($filter) {
      $dbor = db_or();
      $dbor->condition('source', '%' . db_like($filter) . '%', 'LIKE');
      $dbor->condition('redirect', '%' . db_like($filter) . '%', 'LIKE');

      $dbq->condition($dbor);
    }

    $dbs = $dbq->execute();

    while ($row = $dbs->fetchAssoc()) {
      if (!$show_selected && $this->has($row['source'])) {
        continue;
      }

      $redirects[$row['source']] = t("Redirect '<em>@source</em>' => '<em>@redirect</em>'.", array(
        '@source' => $row['source'],
        '@redirect' => $row['redirect'],
      ));
    }

    return $redirects;
  }
}
