/**
 * @file
 * Defining behaviour for the user scripts and history entries table.
 */

(function ($) {
  'use strict';

  Drupal.behaviors.phpx_tools = {
    attach: function (context, settings) {
      $('a.script-load').once().click(function (e) {
        e.preventDefault();

        var $parent = $(this).parents('tr').eq(0);
        var $loadLink = $('.script-load', $parent);
        var $codeCell = $('#' + $loadLink.attr('rel'), $parent);
        var $script_name = $('#edit-script-name');

        if (typeof codemirror != 'undefined' && typeof codemirror.editor != 'undefined') {
          codemirror.editor.setValue($codeCell.text());
        }
        else {
          $('#edit-code').val($codeCell.text());
        }

        if ($script_name && $('#' + $loadLink.attr('rel') + '-name', $parent)) {
          $script_name.val($('#' + $loadLink.attr('rel') + '-name', $parent).text());
        }
      });
    }
  };

})(jQuery);
