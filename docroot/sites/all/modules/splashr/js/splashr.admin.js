(function ($) {

  Drupal.behaviors.splashrAdmin = {
    attach: function (context) {

      var showContentTab = function () {
        var tab = $('#edit-' + $('#edit-splashr-type').val() + '-content').data('verticalTab');
        tab.tabShow();
      };

      // Show proper content tab on page load.
      showContentTab();

      // Switch content tab on changing selection in general settings.
      $("#edit-splashr-type").change(function () {
        showContentTab();
      });

      // Make sure the rest of this behavior
      // is processed only if drupalSetSummary is defined.
      if (typeof jQuery.fn.drupalSetSummary == 'undefined') {
        return;
      }

      // Settings: General tab.
      $('fieldset#edit-general', context).drupalSetSummary(function (context) {
        var vals = [];
        if ($('input[name="splashr_enabled"]:checked', context).val()) {
          vals.push(Drupal.t('Enabled'));
        }
        else {
          vals.push(Drupal.t('Disabled'));
        }
        var selected = $('select[name="splashr_type"]').val();
        vals.push(Drupal.t($('select[name="splashr_type"] option[value='+selected+']').text()));
        return vals.join(', ');
      });

      // Settings: Position tab.
      $('fieldset#edit-position', context).drupalSetSummary(function (context) {
        var vals = [];
        vals.push(Drupal.t('Position: @position', { '@position' : $('select[name="splashr_position"]').val() }));
        vals.push(Drupal.t('Top offset: @offsetpx', { '@offset' : $('input[name="splashr_offset_top"]').val() }));
        return vals.join(', ');
      });

      // Settings: Cookie tab.
      $('fieldset#edit-cookie', context).drupalSetSummary(function (context) {
        var vals = [];
        if ($('input[name="splashr_cookie_disable"]:checked', context).val()) {
          vals.push(Drupal.t('Disabled'));
        }
        else {
          vals.push(Drupal.t('Enabled'));
          var validity = $('input[name="splashr_cookie_validity"]').val();
          if (validity) {
            vals.push(Drupal.t('Validity: @validity days', { '@validity' : validity }))
          }
        }
        return vals.join(', ');
      });

      // Settings: Overlay tab.
      $('fieldset#edit-overlay', context).drupalSetSummary(function (context) {
        var vals = [];
        if ($('input[name="splashr_overlay"]:checked', context).val()) {
          vals.push(Drupal.t('Enabled'));
          var opacity = $('input[name="splashr_overlay_opacity"]').val();
          if (opacity) {
            vals.push(Drupal.t('Opacity: @opacity%', { '@opacity' : opacity }))
          }
        }
        else {
          vals.push(Drupal.t('Disabled'));
        }
        return vals.join(', ');
      });

      // Settings: Dismiss tab.
      $('fieldset#edit-dismiss', context).drupalSetSummary(function (context) {
        var vals = [];
        if ($('input[name="splashr_dismiss_click"]:checked', context).val()) {
          vals.push(Drupal.t('click'));
        }
        if ($('input[name="splashr_dismiss_timeout"]:checked', context).val()) {
          vals.push(Drupal.t('timeout'));
        }
        if ($('input[name="splashr_dismiss_link"]:checked', context).val()) {
          vals.push(Drupal.t('link'));
        }
        if (vals.length > 0) {
          return Drupal.t('Dismiss on: @dismiss', { '@dismiss' : vals.join(', ') });
        }
      });

      // Content: Flash tab.
      $('fieldset#edit-flash-content', context).drupalSetSummary(function (context) {
        var vals = [];
        if ($('input[name="splashr_flash_path"]').val()) {
          vals.push($('input[name="splashr_flash_path"]').val());
        }
        var width = $('input[name="splashr_flash_width"]').val();
        if (width) {
          vals.push(Drupal.t('Width: @widthpx', { '@width' : width }));
        }
        var height = $('input[name="splashr_flash_height"]').val();
        if (height) {
          vals.push(Drupal.t('Height: @heightpx', { '@height' : height }));
        }
        return vals.join(', ');
      });

      // Content: Image background tab.
      $('fieldset#edit-image-content', context).drupalSetSummary(function (context) {
        return $('input[name="splashr_image_path"]').val();
      });

      // Content: HTML tab.
      $('fieldset#edit-html-content', context).drupalSetSummary(function (context) {
        var html = $('textarea[name="splashr_html"]').val();
        if (html.length > 20) {
          html = html.substring(0, 20) + '...';
        }
        var div = document.createElement('div');
        div.appendChild(document.createTextNode(html));
        return div.innerHTML;
      });

      // Visibility: Pages tab.
      $('fieldset#edit-visibility-pages', context).drupalSetSummary(function (context) {
        var $radio = $('input[name="splashr_visibility_pages"]:checked', context);
        if ($radio.val() == 0) {
          if (!$('textarea[name="splashr_pages"]', context).val()) {
            return Drupal.t('Not restricted');
          }
          else {
            return Drupal.t('All pages with exceptions');
          }
        }
        else {
          return Drupal.t('Restricted to certain pages');
        }
      });

      // Visibility: Roles tab.
      $('fieldset#edit-visibility-roles', context).drupalSetSummary(function (context) {
        var vals = [];
        $('input[type="checkbox"]:checked', context).each(function () {
          vals.push($.trim($(this).next('label').text()));
        });
        if (!vals.length) {
          return Drupal.t('Not restricted');
        }
        else if ($('input[name="splashr_visibility_roles"]:checked', context).val() == 1) {
          return Drupal.t('Excepted: @roles', {'@roles' : vals.join(', ')});
        }
        else {
          return vals.join(', ');
        }
      });

    }
  };

})(jQuery);
