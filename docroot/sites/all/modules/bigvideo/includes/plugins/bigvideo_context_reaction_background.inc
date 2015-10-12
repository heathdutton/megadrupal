<?php
/**
 * @file
 * Bigvideo background context reaction.
 */

/**
 * Class bigvideo_context_reaction_background
 *   Integration with Context module. Reaction for add video by context.
 */
class bigvideo_context_reaction_background extends context_reaction {

  /**
   * Reaction options form.
   */
  function options_form($context) {
    $sources = bigvideo_source_load_all();
    $options = array();
    foreach ($sources as $source) {
      $options[$source->bsid] = $source->name;
    }
    $values = $this->fetch_from_context($context);
    $form = array(
      '#title' => t('Background video'),
      'source' => array(
        '#title' => t('Video source'),
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => isset($values['source']) ? $values['source'] : '',
      ),
    );
    if (!$options) {
      $form['source']['#suffix'] = t('You need to !create first', array('!create' => l(t('create source'), 'admin/config/user-interface/bigvideo/sources')));
    }
    return $form;
  }

  /**
   * Attach bigvideo to provided content.
   */
  function execute(&$content) {
    $contexts = context_active_contexts();
    foreach ($contexts as $context) {
      if (!empty($context->reactions['bigvideo'])) {
        $bsid = $context->reactions['bigvideo']['source'];
        if ($source = bigvideo_source_load($bsid)) {
          $links = bigvideo_create_links($source);
          bigvideo_attach_scripts($content, $links);
        }
        else {
          watchdog('BigVideo', 'Attempting to use a non-existent source.', array(), WATCHDOG_ERROR);
        }
      }
    }
  }
}
