/**
 * @file
 * Provides JavaScript functions to the embridge module.
 *
 */
(function ($) {
  Drupal.behaviors.entermedia_asset_preview = {
    attach: function() {            
      $('#preview-dialog').dialog({
        autoOpen: false,
        modal: true,
        height: 540,
        width: 680,
        draggable: false,
        resizable: false,
        closeOnEscape: true,
        zIndex: 9999,
      });
      $('.embridge-asset-search-result .asset-row img').click(function() {
          var parent = $(this).parents('.asset-row');
          var preview = $('input[name=preview]', parent).val();
          $('#preview-dialog').html('<img src="' + preview + '"/>').dialog('open');
      });
      $('#embridge-asset-upload-form .uploaded-asset img').click(function() {
          var parent = $(this).parents('.uploaded-asset');
          var preview = $('input[name=preview]', parent).val();
          $('#preview-dialog').html('<img src="' + preview + '"/>').dialog('open');
      });
    }
  }
  Drupal.behaviors.embridge_save_search = {
    attach: function() {
      $('#edit-asset-search-save').click(function() {
          var assets = new Array();
          $('input:checked').each(function(n, checked) {
            assets[n] = new Array();
            asset_row = $(this).parents('.asset-row');
            asset_row.find(':input').each(function() {
              assets[n][$(this).attr('name')] = $(this).val();
            });
          });
          // Parent page need to define entermedia_process_assets() method
          // in order further process assets from search.
          if(parent && typeof(parent.entermedia_process_search_result) == 'function') {
              parent.entermedia_process_search_result(assets);
          }
          else {
            alert(Drupal.t('Please define function entermedia_process_search_result(assets) on parent page for integration.'));
          }
          return false;
        });
    }
  };

    Drupal.behaviors.embridge_cancel_search = {
      attach: function() {
        $('#edit-cancel').click(function () {
        if(parent && typeof(parent.entermedia_cancel_search) == 'function') {
            parent.entermedia_cancel_search();
        }
        return false;
        });
      }
    };

    Drupal.behaviors.embridge_test_connection = {
      attach: function() {
        $('#edit-test-connection').click(function () {
        var server = $('#embridge-settings').find('input[name="embridge_server_url"]').val();
        var port = $('#embridge-settings').find('input[name="embridge_server_port"]').val();
        var login = $('#embridge-settings').find('input[name="embridge_login"]').val();
        var password = $('#embridge-settings').find('input[name="embridge_password"]').val();
        var url = '/embridge/test-connection';
          $.post(url, {
            server: server,
            port: port,
            login: login,
            password: password
            },
            function(response) {
              alert(response.status);
            }, "json");
        return false;
        });
      }
    };

})(jQuery);

function embridge_get_selected_assets() {
  var assets = new Array();
  jQuery('input:checked').each(function(n, checked) {
    assets[n] = new Array();
    asset_row = jQuery(this).parents('div.asset-row,tr.asset-row');
    asset_row.find(':input').each(function() {
      assets[n][jQuery(this).attr('name')] = jQuery(this).val();
    });
  });
  return assets;
}

function embridge_get_uploaded_asset() {
  asset = new Array();
  jQuery('.uploaded-asset input').each(function() {
    asset[jQuery(this).attr('name')] = jQuery(this).val();
  });
  return asset;
}
