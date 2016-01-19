
/**
 * Behavior to add "Insert" buttons.
 */
(function ($) {
Drupal.behaviors.SyntaxhighlighterInsertText = {
  attach: function(context) {

    // Add the click handler to the insert button.
    if(!(typeof(Drupal.settings.syntaxhighlighter_insert) == 'undefined')){
      for(var key in Drupal.settings.syntaxhighlighter_insert.buttons){
        $("#" + key, context).click(insert);
      }
    }

    function insert() {
      var field = $(this).attr('id');
      var title = $('#' + field.replace('button', 'title')).val()
      var brush = $('#' + field.replace('button', 'brush')).val();
      var tag = $('#' + field.replace('button', 'tag')).val();
      var autolinks = $('#' + field.replace('button', 'auto-links')).is(':checked');
      var classname = $('#' + field.replace('button', 'class-name')).val();
      var collapse = $('#' + field.replace('button', 'collapse')).is(':checked');
      var firstline = $('#' + field.replace('button', 'first-line')).val();
      var highlight = $('#' + field.replace('button', 'highlight')).val();
      var htmlscript = $('#' + field.replace('button', 'html-script')).is(':checked');
      var smarttabs = $('#' + field.replace('button', 'smart-tabs')).is(':checked');
      var tabsize = $('#' + field.replace('button', 'tab-size')).val();
      var toolbar = $('#' + field.replace('button', 'toolbar')).is(':checked');
      var wrapper = $('#' + field.replace('button', 'form-wrapper'));
      var content = '<' + tag + ' class="';
      content += 'brush: ' + brush + '; ';
      content += 'auto-links: ' + new Boolean(autolinks).toString() + '; ';
      if (classname.length) content += "class-name: '" + classname + "'; ";
      content += 'collapse: ' + new Boolean(collapse).toString() + '; ';
      if (firstline.length) content += 'first-line: ' + firstline + '; ';
      if (highlight.length) content += 'highlight: ' + highlight + '; ';
      content += 'html-script: ' + new Boolean(htmlscript).toString() + '; ';
      content += 'smart-tabs: ' + new Boolean(smarttabs).toString() + '; ';
      if (tabsize.length) content += 'tab-size: ' + tabsize + '; ';
      content += 'toolbar: ' + new Boolean(toolbar).toString();
      content += '" ';
      if (title.length) content += 'title="' + title + '" ';
      content += '></' + tag + '>';
      wrapper.dialog('close');
      var pos = content.indexOf('</' + tag + '>');
      var $field = $('#' + Drupal.settings.syntaxhighlighter_insert.buttons[field]);
      Drupal.syntaxhighlighterinsert.insertAtCursor($field, content);
      $field.setCursorToPos(pos);
      return false;
    }

  }
};

Drupal.behaviors.SyntaxhighlighterDialog = {
  attach: function (context) {
    jQuery('.syntaxhighlighter-insert-text-form-wrapper').each(function() {
      $(this).dialog({
        autoOpen: false,
        width: '70%',
        modal: true,
        title: Drupal.t('INSERT SYNTAXHIGHLIGHTER TAG')
      })
    });

    $(".syntaxhighlighter-insert-text-form-link").click(openDialog);

    Drupal.syntaxhighlighterinsert.hideDescriptions();

    function openDialog() {
      var id = '#' + $(this).attr('id').replace('link', 'wrapper');
      $(id + ' .description').hide();
      $(id).dialog('open');
      return false;
    }
  }
}
})(jQuery);

