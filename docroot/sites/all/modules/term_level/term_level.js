(function ($) {

Drupal.behaviors.TermLevel = {
  attach: function (context, settings) {
    if (settings.termLevel instanceof Array) {
      for (var i = 0; i < settings.termLevel.length; i++) {
        var table = $('#' + settings.termLevel[i]);
        Drupal.attachTermLevelTagCloud(table);
        Drupal.attachTermLevelRemoveLinks(table);
        Drupal.attachTermLevelMore(table);
      }
    }
  }
};

Drupal.attachTermLevelTagCloud = function(table) {
  var tagCloud = $(table).find('tr.term-level-element-tag-cloud-row');
  $(tagCloud).once('tagCloud', function() {
    // If a link gets clicked, the hidden ajax form with the link values gets triggered.
    $(tagCloud).find('a.term-level-tag-cloud-links').click(function() {
      $(this).after(Drupal.getTermLevelThrobber());
      var tid = this.id.split('-').pop();
      $(table).find('select.form-select').val(tid);
      $(table).find('input.form-submit').click();
      return false;
    });
  });
}

Drupal.attachTermLevelRemoveLinks = function(table) {
  var rows = $(table).find('tr.term-level-element-table-row');
  $(rows).once('termLevelLink', function() {
    $(this).find('a.term-level-remove-links').click(function() {
      $(this).after(Drupal.getTermLevelThrobber());
      $(this).parents('tr:first').addClass('term-level-element-table-row-none');
      $(this).parents('td:first').find('input.form-radio').attr("checked", "checked");
      $(table).find('select.form-select').val(0);
      $(table).find('input.form-submit').click();
      return false;
    });
  });
}

Drupal.attachTermLevelMore = function(table) {
  $(table).find('a.term-level-tag-cloud-more').click(function() {
    $(this).hide();
    $(table).find('span.term-level-tag-cloud-hide').fadeIn('fast');
    return false;
  });
}

Drupal.getTermLevelThrobber = function() {
  return  $('<span class="ajax-progress ajax-progress-throbber"><span class="throbber">&nbsp;&nbsp;&nbsp;&nbsp;</span></span>');
}

})(jQuery);