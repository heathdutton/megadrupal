<?php
/**
 * @file
 * Implementation of Janrain Social Login widgets.
 */

/**
 * Implements hook_block_view().
 */
function janrain_widgets_block_view($delta = '') {
  if ('social_login' != $delta) {
    return array();
  }
  $sdk = JanrainSdk::instance();
  if (!$sdk->EngageWidget) {
    return array(
      'content' => array(
        '#type' => 'markup',
        '#markup' => t('Janrain Social Login not configured.'),
      ),
    );
  }
  $js = "// @todo move to drupal behaviors?
    function janrainPlexLogin(accessToken) {
      var base_path = Drupal.settings.basePath;
      if (!Drupal.settings.janrain.clean_url) {
        base_path = Drupal.settings.basePath + '?q=';
      } 
      if (!accessToken) {
          console && console.error('Bork!');
      }
      jQuery.ajax({
        url: base_path + 'services/session/token',
        error: function (jqxhr, status, error) {console.error(error);},
        success: function (drupalToken) {
            console && console.log(drupalToken);
            jQuery.ajax({
                url: base_path + 'janrain/login/token.json',
                type:'post',
                xhrFields:{withCredentials:true},
                beforeSend: function (req) {req.setRequestHeader('X-CSRF-Token', drupalToken);},
                error: function (jqxhr, status, error) {console.error(error);},
                data:{token:accessToken},
                success: function (resp) {
                  console.log(resp);                 
                  // @todo generate form from Forms API to gain security and stuff
                  document.getElementById('user_login').submit();
                } // janrain success
            }); // janrain ajax
        } // drupal success
      }); // drupal ajax
    }\n";
  $js .= $sdk->renderJs();
  $js .=
    "janrain.settings.beforeJanrainWidgetOnLoad = [function () {
      janrain.events.onProviderLoginToken.addHandler(function (evt) {
        janrainPlexLogin(evt.token);
      });
    }];\n";
  $block = array('content' => array('#type' => 'markup'));
  $block['content']['#attached']['js'][] = array('type' => 'inline', 'data' => $js);
  foreach ($sdk->getJsSrcs() as $src) {
    $block['content']['#attached']['js'][] = array(
      'type' => 'external', 'group' => JS_LIBRARY, 'data' => $src);
  }
  $block['content']['#markup'] = $sdk->EngageWidget->getHtml();

  $clean_url = variable_get('clean_url', FALSE);
  if ($clean_url) {
    $form_action = $GLOBALS['base_url'] . "/user/login";
  }
  else {
    $form_action = $GLOBALS['base_url'] . "/?q=user/login";
  }
  $block['content']['#markup'] .= '<form action="' . check_url($form_action) . '" style="display:none;" method="post" id="user_login"><input name="form_id" value="user_login"/></form>';
  return $block;
}

/**
 * Implements hook_block_info().
 */
function janrain_widgets_block_info() {
  $blocks = array();
  $blocks['social_login']['info'] = t('Janrain Social Login Block');
  return $blocks;
}

/**
 * Implements hook_preprocess_link().
 */
function janrain_widgets_preprocess_link(&$items) {
  if ($items['path'] == 'user/logout' && JanrainSdk::instance()->FederateWidget) {
    $items['options']['attributes']['onclick'][] = 'window.janrain.plex.ssoLogout();';
  }
}
