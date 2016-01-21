(function ($) {
/**
 * Tests drag & drop.
 */
Drupal.tests.dragdrop = {
  getInfo: function() {
    return {
      name: 'Drag and drop',
      description: 'Tests for drag and drop.',
      group: 'System',
      useSimulate: true
    };
  },
  tests: {
    dragdrop: function ($, Drupal) {
      return function() {
        expect(3);
        var $dragme = $('tr.region-title-help').next().next().find('a.tabledrag-handle').first();
        var $dragto = $('tr.region-title-header');

        var dragmeOffset = $dragme.offset().top;
        var dragtoOffset = $dragto.offset().top;

        // Check that 'System help' is in region 'Help'.
        $('#edit-blocks-system-help-region').val('help');
        equal($('#edit-blocks-system-help-region').val(), 'help', Drupal.t('"System help" in region "Help"'));

        // Drag to 'Header'
        $dragme.simulate("drag", {
          dx: 0,
          dy: dragmeOffset - dragtoOffset + 50
        });

        // Check that 'System help' is in region 'Content'.
        equal($('#edit-blocks-system-help-region').val(), 'content', Drupal.t('"System help" in region "Content"'));

        // Check for the presence of the warning.
        equal($('div.messages.warning').length, 1, Drupal.t('Warning message found.'));
      }
    }
  }
};

})(jQuery);