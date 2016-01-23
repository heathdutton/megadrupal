/**
 * @file instantclick.js
 */
(function ($) {

Drupal.instantclick = Drupal.instantclick || {};

$(window).load( function() {
  InstantClick.on('change', Drupal.instantclick.pageLoad);
  if (Drupal.settings.instantClickConfig.instantClickMode == 'whitelist') {
    InstantClick.init(true);
  }
  else {
    InstantClick.init(false);
  }
});


Drupal.instantclick.pageLoad = function() {

  // Determine which stylesheets have already been loaded.
  var styleSheetsPresent = [];
  $.each(document.styleSheets, function(key, val) {
    if (val.media[0] == 'all' || val.media[0] == 'screen') {
      if (val.href) {
        styleSheetsPresent.push(val.href);
      }
      else {
        $.each(val.cssRules, function (ruleKey, ruleVal) {
          if (ruleVal.href) {
            var sheetPath = document.createElement('a');
            sheetPath.href = ruleVal.href;
            styleSheetsPresent.push(sheetPath.pathname);
          }
        });
      }
    }
  });

  var styleSheetsNeeded = [];
  // Load any additional stylesheets needed for this page.
  $.each(Drupal.settings.instantClickInfo.css, function(key, val) {
    if (val.browsers['!IE'] && val.type == 'file' && ((val.media == 'all') || (val.media == 'screen'))) {
      styleSheetsNeeded.push(val.data);
      if (styleSheetsPresent.indexOf(val.data) == -1) {
        $('head').append( $('<link rel="stylesheet" type="text/css">').attr('href', val.data) );
      }
    }
  });

  // Remove stylesheets that aren't supposed to be present on this page..
  $.each(styleSheetsPresent, function(key, val) {
    if (styleSheetsNeeded.indexOf(val) == -1) {
      $('link[rel=stylesheet][href~="' + val + '"]').remove();
    }
  });

  // Determine which javascript files have already been loaded.
  var scriptsPresent = [];
  $.each($('script'), function(key, val) {
    scriptsPresent.push($(this).attr('src'));
  });

  // Load any additional javascript needed for this page.
  var scriptsNeeded = [];
  $.each(Drupal.settings.instantClickInfo.js, function(key, val) {
    if (val.type == 'file') {
      scriptsNeeded.push(val.src);
      if (scriptsPresent.indexOf(val.src) == -1) {
        $('head').append( $('<script type="text/javascript">').attr('src', val.src) );
      }
    }
    else if (val.type == 'setting' || val.type == 'inline') {
      eval(val.script);
    }
  });

  // TODO: Remove javascript that isn't supposed to be present on this page..
  // $.each(scriptsPresent, function(key, val) {
  //   if (scriptsNeeded.indexOf(val) == -1) {
  //   }
  // });

  // Attach registered behaviors to the page that has been loaded via AJAX.
  Drupal.attachBehaviors();

}

})(jQuery);
