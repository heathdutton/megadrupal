<?php
/**
 * @file
 * Source: GeoJson.
 */

namespace Drupal\openlayers\Plugins\Source\GeoJSON;
use Drupal\Component\Annotation\Plugin;
use Drupal\openlayers\Types\Source;
use Drupal\openlayers\Config;

/**
 * Class GeoJSON.
 *
 * @Plugin(
 *  id = "GeoJSON"
 * )
 *
 */
class GeoJSON extends Source {

  /**
   * {@inheritdoc}
   */
  public function defaultProperties() {
    $default_properties = parent::defaultProperties();
    $default_properties['options']['paramForwarding'] = TRUE;
    return $default_properties;
  }

  /**
   * {@inheritdoc}
   */
  public function optionsForm(&$form, &$form_state) {
    $form['options']['url'] = array(
      '#title' => t('URL'),
      '#type' => 'textfield',
      '#default_value' => $this->getOption('url'),
    );
    $form['options']['useBBOX'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use Bounding Box Strategy'),
      '#description' => t('Bounding Box strategy will add a query string onto the GeoJSON URL, which means that only data in the viewport of the map will be loaded.  This can be helpful if you have lots of data coming from the feed.'),
      '#default_value' => $this->getOption('useBBOX'),
    );
    $form['options']['paramForwarding'] = array(
      '#type' => 'checkbox',
      '#title' => t('Forward parameters on bbox load'),
      '#description' => t('If enabled all GET request parameters will be forwarded when loading the bbox content.'),
      '#default_value' => $this->getOption('paramForwarding', TRUE),
      '#states' => array(
        'invisible' => array(
          ':input[name="options[useBBOX]"]' => array('checked' => FALSE),
        ),
      ),
    );
    $form['options']['reloadOnZoomChange'] = array(
      '#type' => 'checkbox',
      '#title' => t('Reload features on zoom change.'),
      '#description' => t('Reload the features if the zoom level of the map changes. Handy if you use a zoom aware backend clustering.'),
      '#default_value' => $this->getOption('reloadOnZoomChange'),
    );
    $form['options']['reloadOnExtentChange'] = array(
      '#type' => 'checkbox',
      '#title' => t('Reload features on extent change'),
      '#description' => t('Reload the features if the visible part of the map changes (e.g. by dragging the map).'),
      '#default_value' => $this->getOption('reloadOnExtentChange'),
    );

//    //see http://dev.openlayers.org/docs/files/OpenLayers/Strategy/BBOX-js.html#OpenLayers.Strategy.BBOX.resFactor
//    $form['options']['resFactor'] = array(
//      '#type' => 'textfield',
//      '#title' => t('Bounding Box resFactor'),
//      '#description' => t('Used to determine when previously requested features are invalid (set to 1 if unsure).
//          The resFactor will be compared to the resolution of the previous request to the current map resolution.<br />
//          If resFactor > (old / new) and 1/resFactor < (old / new).
//          <ul>
//          <li>If you set a resFactor of 1, data will be requested every time the resolution changes.</li>
//          <li>If you set a resFactor of 3, data will be requested if the old resolution is 3 times the new, or if the new is 3 times the old.</li>
//          <li>If the old bounds do not contain the new bounds new data will always be requested (with or without considering resFactor).</li>
//          </ul>
//          '),
//      '#default_value' => $this->getOption('resFactor', 1),
//    );
//  //see hZttp://dev.openlayers.org/docs/files/OpenLayers/Strategy/BBOX-js.html#OpenLayers.Strategy.BBOX.ratio
//    $form['options']['ratio'] = array(
//      '#type' => 'textfield',
//      '#title' => t('Bounding Box ratio'),
//      '#description' => t('The ratio of the data bounds to the viewport bounds (in each dimension).  Default is 3.'),
//      '#default_value' => $this->getOption('ratio', 3),
//    );
//    $form['options']['preload'] = array(
//      '#type' => 'checkbox',
//      '#title' => t('Preload layer'),
//      '#description' => t('Load data before layer is made visible. Useful when you want to avoid having to wait for an Ajax call to load the data'),
//      '#default_value' => $this->getOption('preload', FALSE),
//    );
//    $form['options']['useScript'] = array(
//      '#type' => 'checkbox',
//      '#title' => t('Use Script Method'),
//      '#description' => t('Avoid 405 error and XSS issues load data from another server with ajax'),
//      '#default_value' => $this->getOption('useScript', FALSE),
//    );
//    $form['options']['callbackKey'] = array(
//      '#type' => 'textfield',
//      '#title' => t('Script Callback Key'),
//      '#description' => t('Key returned by callback along with geoJSON'),
//      '#default_value' => $this->getOption('callbackKey', NULL),
//    );
    $form['options']['geojson_data'] = array(
      '#type' => 'textarea',
      '#title' => t('GeoJSON Data'),
      '#description' => t('Paste raw GeoJSON data here. It is better to use a URL.  This is provided for very simple use cases like one or two features.  If there is data here, it will override the URL above.'),
      '#default_value' => $this->getOption('geojson_data'),
      '#states' => array(
        'invisible' => array(
          ':input[name="options[useBBOX]"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form['options']['devMode'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable developer mode.'),
      '#description' => t('If enabled you can edit the request to send using a dialog.'),
      '#default_value' => $this->getOption('devMode'),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getJS() {
    $js = parent::getJS();
    // Ensure we've a sane url.
    if (!empty($js['opt']['url'])) {
      $js['opt']['url'] = url($js['opt']['url']);
    }
    else {
      // Remove the option as it is even used if empty.
      unset($js['opt']['url']);
    }

    // @TODO Find a way how to do this just once per map / collection.
    if ($this->getOption('devMode')) {
      $form_state = array();
      $form_state['build_info']['args'] = array($this);
      $form = drupal_build_form('openlayers_dev_dialog_form', $form_state);
      unset($form['options']['devMode']);
      $js['opt']['devDialog'] = filter_xss(
        drupal_render($form),
        array(
          'label',
          'form',
          'input',
          'select',
          'textarea',
          'div',
          'ul',
          'ol',
          'li',
          'dl',
          'dt',
          'dd',
        )
      );
    }

    return $js;
  }

  /**
   * {@inheritdoc}
   */
  public function attached() {
    $attached = parent::attached();
    $plugin = $this->getConfiguration();
    $plugin['path'] = $this->getClassDirectory();
    if ($this->getOption('devMode')) {
      // @TODO Find a way how to do this just once per map / collection.
      $attached['library']['system.ui.dialog'] = array('system', 'ui.dialog');
      $attached['library']['system.jquery.cookie'] = array('system', 'jquery.cookie');
      $attached['js'][$plugin['path'] . '/js/geojson_dev.js']['weight'] = Config::get('openlayers.js_css.weight') + 1;
    }
    else {
      unset($attached['js'][$plugin['path'] . '/js/geojson_dev.js']);
      unset($attached['css'][$plugin['path'] . '/css/geojson_dev.css']);
    }
    return $attached;
  }
}
