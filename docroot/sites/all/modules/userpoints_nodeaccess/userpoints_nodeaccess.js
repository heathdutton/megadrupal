// $Id: userpoints_nodeaccess.js,v 1.2 2010/11/23 14:08:43 berdir Exp $
(function ($) {
  Drupal.behaviors.userpointsNodeAccessFieldsetSummaries = {
    attach: function (context) {
      $('fieldset.userpoints-nodeaccess-form', context).drupalSetSummary(function (context) {
        if ($('#edit-userpoints-nodeaccess-points-price', context).val() > 0) {
          var params = {
            '@points': $('#edit-userpoints-nodeaccess-points-price', context).val(),
            '@category': $('select#edit-userpoints-nodeaccess-points-category :selected', context).text()
          };
          return Drupal.t('@points in category @category required.', params);
        }
        else {
          return Drupal.t('No points required.');
        }
      });
    }
  };
  Drupal.behaviors.userpointsNodeAccessNodeTypeFieldsetSummaries = {
    attach: function (context) {
      $('fieldset.userpoints-nodeaccess-nodetype-form', context).drupalSetSummary(function (context) {
        if ($('#edit-userpoints-nodeaccess-enabled').is(':checked')) {
          if ($('#edit-userpoints-nodeaccess-points-price', context).val() > 0) {
            var params = {
              '@points': $('#edit-userpoints-nodeaccess-points-price', context).val(),
              '@category': $('select#edit-userpoints-nodeaccess-points-category :selected', context).text()
            };
            return Drupal.t('Userpoints node access enabled. @points in category @category required.', params);
          }
          else {
            return Drupal.t('Userpoints node access enabled. No points required by default.');
          }
        }
        else {
          return Drupal.t('Userpoints node access not enabled.');
        }
      });
    }
  };

})(jQuery);
