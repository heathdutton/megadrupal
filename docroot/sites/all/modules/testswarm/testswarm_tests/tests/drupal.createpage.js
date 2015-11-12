/*jslint vars: true, indent: 2, white: true */
/*global Drupal: true, jQuery: true, window: true */
(function ($, Drupal) {
  "use strict";
  Drupal.tests.createpagecontent = {
    getInfo: function() {
      return {
        name: 'Menu settings',
        description: 'Tests for vertical tabs summary.',
        group: 'System'
        // @TODO: fix permissions first waitForPageLoad: true
      };
    },
    tests: {
      menutitlevisible: function ($, Drupal) {
        return function() {
          expect(2);

          // Find the menu vertical tab and click
          var menutab = '.vertical-tabs li:contains("Menu")';
          $(menutab).find('a').first().trigger('click');

          // Click on the checkbox.
          $('#edit-menu-enabled')
            .trigger('click')
            .trigger('change');

          equal($('#edit-menu-link-title:visible').length, 1, Drupal.t('Menu title visible'));

          // Fill in a title
          $('#edit-menu-link-title')
            .val('xyzzy')
            .trigger('change');

          // Check if summary is set
          equal($(menutab).find('span.summary:contains("xyzzy")').length, 1, Drupal.t('Menu summary found'));

          $('#edit-title').val('test');
          $('#edit-field-test-und-0-value').val('test');

          // @TODO: fix permissions first $('#page-node-form').submit();
        }
      }
      /*
       * @TODO: fix permissions first
      ,
      checktitle: function ($, Drupal) {
        return function() {
          expect(1);
          equal($('#page-title').text().replace(/[\s+|\n]/g, ''), 'test', 'Menu title set correctly');
          TestSwarm.gotoURL (TestSwarm.getURL() + '/edit');
        }
      }
      */
    }
  };
})(jQuery, Drupal);