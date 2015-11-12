<?php

/**
 * @file
 * Definition of views_handler_field_applenews_previewable.
 */

/**
 * A handler to provide proper display for Apple News previable download link.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_applenews_previewable extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  /**
   * Provide link to the page being visited.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }

  function render($values) {
    $articleId = $this->get_value($values);
    $post = new ApplenewsDatabase();
    $article = $post->getArticleById($articleId);
    if (!empty($article['entity_id'])) {
      return l(t('preview'), 'admin/config/content/applenews/' . $article['entity_type'] . '/' . $article['entity_id'] . '/' . $article['revision_id'] . '/download',
        [
          'attributes' => [
            'title' => t('Please use News Preview app to preview this article.')
          ]
        ]
      );
    }
    return NULL;
  }

}
