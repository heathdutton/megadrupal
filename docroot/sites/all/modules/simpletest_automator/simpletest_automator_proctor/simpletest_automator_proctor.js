
/**
 * Convert clicking on links to $this->clickLink().
 */
Drupal.behaviors.trapSimpleTestClick = function() {
  $('a[href]:not(.trappedSimpleTestClick)').each(function() {
    $(this).addClass('trappedSimpleTestClick').click(function() {
      var content = $(this).text();
      var elems = $('a[href]').filter(function() {
        return $(this).text() == content;
      });

      for (var i = 0; i < elems.length; i++) {
        if (elems[i] == this) {
          break;
        }
      }

      $('body').css('cursor', 'wait');
      if (simpleTestRecord({'type': 'click_link', 'label': content, 'index': i})) {
        window.location = $(this).attr('href') + ($(this).attr('href').indexOf('?') == -1 ? '?record=false' : '&record=false');
        return false;
      }
    });
  });
};

/**
 * When we've selected text, give the option to check for that text.
 */
Drupal.behaviors.recordSimpleTestText = function() {
  $('body').mouseup(function() {
    var selected = window.getSelection ? window.getSelection() : (document.getSelection ? document.getSelection() : (document.selection ? document.selection.createRange().text : ''));
    selected = selected.toString();
    if (selected) {
      simpleTestRecord({'type': 'assert_text', 'text': selected, 'html': false, 'should_appear': true});
    }
    return true;
  });
};

/**
 * Attach the control panel.
 */
Drupal.behaviors.simpleTestControlPanel = function() {
  $('body').append('<div id="control-panel" class="display-inline" />');
  $('#control-panel').append('<label class="option"><input type="checkbox" class="form-checkbox" value="control-panel-auto-record" id="edit-control-panel-auto-record" name="control_panel"/> Auto-Record</label>');
  $('#control-panel').append('<label class="option"><input type="checkbox" class="form-checkbox" value="control-panel-stop-recording" id="edit-control-panel-stop-recording" name="stop_recording"/> Stop</label>');
  $('#control-panel').css('float', 'left').css('position', 'fixed').css('bottom', '0px').css('right', '0px').css('padding', '3px').css('background', '#ddd');
  $('#edit-control-panel-stop-recording').change(function() {
    if ($(this).is(':checked')) {
      if (confirm(Drupal.t('Are you sure you\'d like to stop recording?'))) {
        $('body').css('cursor', 'wait');
        simpleTestRecord({'type': 'end'});
        document.cookie = 'simpletest_automator=';
        window.location = Drupal.settings.basePath + 'admin/build/simpletest_automator';
      }
    }
    $(this).removeAttr('checked');
    return false;
  });
};

/**
 * Record some SimpleTest code to PHP.
 */
function simpleTestRecord(args) {
  if (args.type != 'end' && !$('#edit-control-panel-auto-record').is(':checked')) {
    result = simpleTestConfirm(args);
    if (result) {
      args = result;
    }
    else {
      return;
    }
  }
  return $.ajax({
    'async': false,
    'cache': false,
    'url': Drupal.settings.basePath + 'simpletest_automator/js',
    'data': args,
    'timeout': 30,
    'error': function() {
      alert('An error has occurred.');
    },
    'type': 'POST',
  });
}

/**
 * Display the confirm form.
 */
function simpleTestConfirm(args) {
  switch (args.type) {
    case 'click_link':
      if (confirm(Drupal.t('Would you like to record your click?'))) {
        return args;
      }
      break;

    case 'assert_text':
      if (confirm(Drupal.t('Would you like to make sure the "!text" text appears?', {'!text': args.text}))) {
        args.should_appear = !!confirm(Drupal.t('Should this text be appearing? Choose "ok" if it should, or "cancel" if it should not.'));
        args.html = false;
        return args;
      }
  }
  return false;
}