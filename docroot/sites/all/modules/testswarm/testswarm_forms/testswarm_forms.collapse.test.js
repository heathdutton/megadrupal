(function ($) {
/**
 * Collapsible fields.
 */
Drupal.tests.autocomplete = {
  getInfo: function() {
    return {
      name: 'Collapsible fields',
      description: 'Tests for Collapsible fields.',
      group: 'System',
      useSimulate: true
    };
  },
  tests: {
    fieldset1: function ($, Drupal) {
      return function() {
        expect(9);
        var collapseDelay = 1000;

        // The first fieldset should be visible initially, but we should be able to
        // toggle it by clicking on the legend.
        ok($('#edit-fieldset1').find('div.fieldset-wrapper').is(':visible'), Drupal.t('First fieldset is initially visible.'));
        ok($('#edit-fieldset1').hasClass('collapsible'), Drupal.t('First fieldset has the "collapsible" class.'));
        ok(!$('#edit-fieldset1').hasClass('collapsed'), Drupal.t('First fieldset does not have the "collapsed" class.'));
        // Trigger the collapse behavior by simulating a click.
        $('#edit-fieldset1').find('legend a').click();
        stop();
        setTimeout(function() {
          ok($('#edit-fieldset1').find('div.fieldset-wrapper').is(':hidden'), Drupal.t('First fieldset is not visible after being toggled.'));
          ok($('#edit-fieldset1').hasClass('collapsible'), Drupal.t('First fieldset has the "collapsible" class after being toggled.'));
          ok($('#edit-fieldset1').hasClass('collapsed'), Drupal.t('First fieldset has the "collapsed" class after being toggled.'));

          // Trigger the collapse behavior again by simulating a click.
          $('#edit-fieldset1').find('legend a').click();
          setTimeout(function() {
            ok($('#edit-fieldset1').find('div.fieldset-wrapper').is(':visible'), Drupal.t('First fieldset is visible after being toggled again.'));
            ok($('#edit-fieldset1').hasClass('collapsible'), Drupal.t('First fieldset has the "collapsible" class after being toggled again.'));
            ok(!$('#edit-fieldset1').hasClass('collapsed'), Drupal.t('First fieldset does not have the "collapsed" class after being toggled again.'));
            start();
          }, collapseDelay);
        }, collapseDelay);        
      }
    },
    fieldset2: function ($, Drupal) {
      return function() {
        expect(9);
        var collapseDelay = 1000;

        // The second fieldset should be initially hidden, but we should be able to
        // toggle it by clicking on the legend.
        ok($('#edit-fieldset2').find('div.fieldset-wrapper').is(':hidden'), Drupal.t('Third fieldset is not initially visible.'));
        ok($('#edit-fieldset2').hasClass('collapsible'), Drupal.t('Third fieldset has the "collapsible" class.'));
        ok($('#edit-fieldset2').hasClass('collapsed'), Drupal.t('Third fieldset has the "collapsed" class.'));
        // Trigger the collapse behavior by simulating a click.
        $('#edit-fieldset2').find('legend a').click();
        stop();
        setTimeout(function() {
          ok($('#edit-fieldset2').find('div.fieldset-wrapper').is(':visible'), Drupal.t('Third fieldset is visible after being toggled.'));
          ok($('#edit-fieldset2').hasClass('collapsible'), Drupal.t('Third fieldset has the "collapsible" class after being toggled.'));
          ok(!$('#edit-fieldset2').hasClass('collapsed'), Drupal.t('Third fieldset does not have the "collapsed" class after being toggled.'));
          $('#edit-fieldset2').find('legend a').click();
          setTimeout(function() {
            // Trigger the collapse behavior again by simulating a click.
            ok($('#edit-fieldset2').find('div.fieldset-wrapper').is(':hidden'), Drupal.t('Third fieldset is not visible after being toggled again.'));
            ok($('#edit-fieldset2').hasClass('collapsible'), Drupal.t('Third fieldset has the "collapsible" class after being toggled again.'));
            ok($('#edit-fieldset2').hasClass('collapsed'), Drupal.t('Third fieldset has the "collapsed" class after being toggled again.'));
            start();
          }, collapseDelay);
        }, collapseDelay);
      }
    },
    fieldset3: function ($, Drupal) {
      return function() {
        expect(6);
        var collapseDelay = 1000;
        
        ok($('#edit-fieldset3').find('div.fieldset-wrapper').is(':visible'), Drupal.t('Second fieldset is initially visible.'));
        ok(!$('#edit-fieldset3').hasClass('collapsible'), Drupal.t('Second fieldset does not have the "collapsible" class.'));
        ok(!$('#edit-fieldset3').hasClass('collapsed'), Drupal.t('Second fieldset does not have the "collapsed" class.'));
        // After toggling, nothing should happen.
        $('#edit-fieldset3').find('legend a').click();
        stop();
        setTimeout(function() {
          ok($('#edit-fieldset3').find('div.fieldset-wrapper').is(':visible'), Drupal.t('Second fieldset is still visible after toggling.'));
          ok(!$('#edit-fieldset3').hasClass('collapsible'), Drupal.t('Second fieldset still does not have the "collapsible" class after toggling.'));
          ok(!$('#edit-fieldset3').hasClass('collapsed'), Drupal.t('Second fieldset still does not have the "collapsed" class after toggling.'));
          start();
        }, collapseDelay);
      }
    },
    fieldset4: function ($, Drupal) {
      return function() {
        expect(6);
        var collapseDelay = 1000;
        
        ok($('#edit-fieldset4').find('div.fieldset-wrapper').is(':visible'), Drupal.t('Second fieldset is initially visible.'));
        ok(!$('#edit-fieldset4').hasClass('collapsible'), Drupal.t('Second fieldset does not have the "collapsible" class.'));
        ok(!$('#edit-fieldset4').hasClass('collapsed'), Drupal.t('Second fieldset does not have the "collapsed" class.'));
        // After toggling, nothing should happen.
        $('#edit-fieldset4').find('legend a').click();
        stop();
        setTimeout(function() {
          ok($('#edit-fieldset4').find('div.fieldset-wrapper').is(':visible'), Drupal.t('Second fieldset is still visible after toggling.'));
          ok(!$('#edit-fieldset4').hasClass('collapsible'), Drupal.t('Second fieldset still does not have the "collapsible" class after toggling.'));
          ok(!$('#edit-fieldset4').hasClass('collapsed'), Drupal.t('Second fieldset still does not have the "collapsed" class after toggling.'));
          start();
        }, collapseDelay);
      }
    }
  }
};
})(jQuery);