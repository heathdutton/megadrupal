/**
 * @file
 * Multilang widget.
 */

(function($) {

var
  path = '',
  isInput = 0, // flag set when the current page contains input areas

  // Configuration settings.
  help = {},
  filters = {},
  multify = {},
  fallback = {},
  
  // Languages info.
  langs = {}, // array of all defined language codes, like ['en'],['fr']
  defLang = '', // default site language code, like "en"
  curLang = '', // current language code, like "fr"
  langNative = '', // something like "...French..." if current language is "fr"
  
  // Input operation.
  $entryArea = {}, // input set of translations (unique for all inputs)
  curInput = undefined, // input element where currently operating
  stack = {}, // input content history
  changeSurvey = {}, // most recent input content
  surveyInProgress = undefined,
  
  // Auxiliary DOM objects and data.
  $showTemplate = {}, // show translations set (will be cloned for each input)
  resizing = {}, // register textarea and showArea while resizing
  $cache = {},
  $dialog = {}, // user prompt for  choosing a language
  chosenLang = '', // user response from prompt
  
  // RegExp operations.
  lookForSegment = new RegExp(
    '(?:\\[|&lt;)multi(?:\\]|&gt;)([\\s\\S]*?)(?:\\[|&lt;)/multi(?:\\]|&gt;)',
    'ig'
  ),
  lookForCustom = new RegExp(
    '\\[multilang-custom\\]', 'g'
  ),
  lookForHighlight = new RegExp(
    '\\[multilang-highlight\\]([\\s\\S]*?)\\[/multilang-highlight\\]','g'
  ),
  langMarkOrEnd = '', // populated in buildWidget()
  
  BR = '<br />',
  NBSP = '&nbsp;',
  PARA = '&para;',
  lookForPara = new RegExp('&(?:amp;)?para;', 'g'),
  
  CTRL = 17,
  ENTER = 13,
  ESC = 27,
  SHIFT = 16,
  TAB = 9,
  lastWhich = undefined,
  
  classGRIPPIE = '.grippie', // (Drupal standard)
  CKEDITORMOD = 'ckeditor-mod'; // (Drupal standard)
  MULTILANG = 'multilang',
  NOOPTAGS = ['LINK', 'META', 'SCRIPT', 'STYLE'],
  
  // Magic constants source.
  names = [
    'switcher', // (own)
    'cache', 'curlang', 'deflang', 'dialog', 'entry', 'entryfirst', 'entrylast',
    'highlight', 'lang', 'link', 'sellang', 'show', 'text', 'undo',
  ],
  datanames = [
    'inputid', 'func', 'selectlang',
  ]
  events = ['blur', 'click', 'keydown', 'mousedown', 'mouseenter', 'mousemove',
    'mouseup',
  ],
  evals = [];
  
  // Create Multilang magic constants.
  buildConstants(names, 'names');
  buildConstants(datanames, 'datanames');
  buildConstants(events, 'events');
  eval('var ' + evals.join(',') + ';');

$(document).ready(function() {

  // If required, stop before execution to allow examining the current content.
  if (location.search == '?stop') {
    debugger;
  }

  // Get configuration settings.
  init();
  
  // Overwrite some Drupal functions.
  alterDrupal();
  
  // Render current language, linebreaks, and highlight (not including title).
  $('html').each(renderElement);
  $(classHIGHLIGHT, 'body').each(setHighlightTitle);
  
  // Then work only if current page contains involved input areas.
  if (isInput) {
    
    // Create needed widget components.
    buildWidget();
    
    // Bind general events.
    bindEvents();
    
    // Attach Multilang widget to input and textarea elements when involved.
    $('.form-text, .form-textarea').not(filters.noWidget).each(initInput);
  }
  
});
//------------------------------------------------------------------- MAIN FUNCS
/**
 * init(): get configuration settings.
 */
function init() {
  var conf = Drupal.settings.multilang;
  
  // Basic configuration.
  help = $.parseJSON(conf.help);
  filters.noWidget = $(conf.filters.noWidget.join(','));
  //filters.useLinebreak = $(conf.filters.useLinebreak.join(','));
  // Above statement replaced by the following as jQuery <= 1.6
  filters.useLinebreak = conf.filters.useLinebreak,
  langs = conf.langs;
  defLang = conf.defLang;
  curLang = conf.curLang;
  langNative = wrap(langs[curLang], '...');
  multify.fromLang = conf.multify.fromlang[0];
  multify.backLang = conf.multify.backlang[0];
  isInput = conf.isInput;
  path = conf.path;
  
  // Fallback configuration.
  fallback.empty = conf.fallback.empty[0];
  fallback.option = conf.fallback.option[0];
  fallback.message = conf.fallback.message;
  fallback.tip = conf.fallback.tip;
  fallback.span = '<span class=' + HIGHLIGHT + '>';
  // (does not include title attribute, set separately)
  
  // Langs list for lookForLang().
  for (var lang in langs) {
    langMarkOrEnd += '\\[' + lang + '\\]|';
  }
  langMarkOrEnd = '(' + langMarkOrEnd + '\\[/multi\\])';
}

/**
 * alterDrupal(): alter some functions where needed.
 */
function alterDrupal() {
  // Overwrite Drupal.verticalTab.updateSummary to localize summary contents.
  if (Drupal.verticalTab) {
    var proto = Drupal.verticalTab.prototype;
    proto.originalUpdateSummary = proto.updateSummary;
    proto.updateSummary = function() {
      proto.originalUpdateSummary.call(this);
      renderElement.call(this.summary);
    };
  }
}

/**
 * renderElement(): render element in the current language, process linebreaks.
 *
 * Recursively processes element to look for "multi" segments and linebreak
 * flags in text nodes (input and textarea elements are never involved).
 */
function renderElement() {
  // "this" is the current element.
  $(this).contents().each(function() {
    switch (this.nodeType) {
      case Node.TEXT_NODE:
        // To improve performance, process current text node only if not empty.
        if (!this.textContent.match(/^[\s]+$/)) {
          renderTextNode(this);
        }
        break;
      case Node.ELEMENT_NODE:
        // To improve performance, don't explore some tags.
        if($.inArray(this.tagName, NOOPTAGS) < 0) {
          // Process title attribute, if any.
          // CAUTION: don't use $(this).attr('title') -> wrong with jQuery 1.4
          // (returns element <input name="title"> belonging to "this" <form>)
          var title = this.getAttribute('title');
          if (!!title) {
            $(this).attr('title', renderString(title, false, false));
          }
          // Explore current child, unless if it is a <textarea>.
          if (this.tagName != 'TEXTAREA') {
            renderElement.call(this);
          }
        }
        break;
      default:
        // Do nothing for any other nodeType.
    }
  });
}

/**
 * setHighlightTitle(): add title for highlighted elements.
 */
function setHighlightTitle() {
  // Don't overwrite if already set (real tip from server side).
  if (!this.getAttribute('title')) {
    this.setAttribute('title', fallback.tip);
  }
}

/**
 * buildWidget(): create needed widget components.
 */
function buildWidget() {
 
  $('.language-switcher-locale-url').addClass(SWITCHER); // (own)
  
  $showTemplate =
    $('<div class="' + SHOW + '" title="' + help.inputTitle + '" />');
  $entryArea =
    $('<table class="' + ENTRY + '" title="' + help.widgetTitle + '" />')
    .appendTo('body');
  $cache = $('<div id="' + CACHE + '" />').appendTo('body');
  $dialog = $('<div id="' + DIALOG + '" />').appendTo('body');
  
  // Populate widget areas and dialog from languages set.
  var langChoice = '';
  for (var lang in langs) {
    var isDefLang = (lang == defLang),
        isCurLang = (lang == curLang);
    $showTemplate.append(wrapTag(lang,
      '<div class="' + LANG + ' ' + LANG + '-' + lang
      + (isDefLang ? (' ' + DEFLANG) : '')
      + (isCurLang ? (' ' + CURLANG) : '')
      + '">'));
    $entryArea.append(
      wrapTag(
        wrapTag(lang, '<th>')
        + wrapTag('<textarea />', '<td>'),
        '<tr class="' + ENTRY + '-' + lang + '">'
      )
    );
    langChoice +=
      '<label><input type="radio" name="' + LANG + '" value="'
      + lang + '" />&nbsp;' + langs[lang]
      + (isDefLang ? ', ' + help.defLang : '')
      + (isCurLang ? ', ' + help.curLang : '')
      + '</label><br />';
  }
  
  // Finalize components building.
  $showTemplate.append('<pre class="' + TEXT + '" />');
  $entryArea.find('textarea:first').addClass(ENTRYFIRST);
  $entryArea.find('textarea:last').addClass(ENTRYLAST);
  langChoice +=
    '<button value="ok">' + help.ok + '</button>&nbsp;&nbsp;'
    + '<button value="cancel">' + help.cancel + '</button>';
  $dialog
  .append('<p class="content" />')
  .append(wrapTag(langChoice, '<p>'));
}

/**
 * bindEvents(): bind general events.
 */
function bindEvents() {

  // When hover language label in showArea, show corresponding text.
  $(classLANG, $showTemplate).bind(MOUSEENTER, mouseenterShow);
    
  // When click language label or text in showArea, open entryArea.
  $showTemplate.bind(CLICK, clickShow);
  
  $('textarea', $entryArea)
    // When {Ctrl-Enter}/{Esc}/{Tab} on entryArea, validate/cancel/keep operation.
    .bind(KEYDOWN, function(event) {
      return keydownEntry(event);
    })
    // When simply leaving entryArea, validate operation.
    .bind(BLUR, function(event) {
      blurEntry(event);
      return false;
    });
  
  // When a textarea is resized, make showArea to follow its height.
  $(classGRIPPIE).bind(MOUSEDOWN, mousedownGrippie);
}

/**
 * initInput(): normalize content and create associated widget.
 */
function initInput() {
  // "this" is the input or textarea element.
  
  if (getInputLabel(this)) { // (or not a really involved input)
    // Normalize legacy <multi> and </multi>, if any.
    this.value = this.value.replace(/<(\/)?multi>/ig,'[$1multi]');
    // Initially assign curLang to selected language.
    this.dataset[ccSELECTLANG] = curLang;
    // Save initial value (will allow to detect changes without change event).
    changeSurvey[this.id] = this.value;
    // Create widget.
    createWidget(this);
  }
}
//-------------------------------------------------------------------- SUB FUNCS
/**
 * attachWidget(): attach/detach showArea to/from the current input element.
 */
function attachWidget(input, doAttach) {
  var $input = $(input),
      $showArea = $(classSHOW, input.parentElement);
  
  if (doAttach) {
    // Ensure language marks to fit into container.
    $showArea.show()
      .css({width:
        // This way allows using jQuery < 1.8.
        $input.show().width() // (first show, or wrong width reported)
        - parseInt($showArea.css('borderLeftWidth') | 0)
        - parseInt($showArea.css('borderRightWidth') | 0)
      })
      // Default display last selected language.
      .find(classLANG + '-' + input.dataset[ccSELECTLANG]).mouseenter();
    $input
      .css({minHeight: $showArea.find(classLANG + ':first').outerHeight()})
      .hide();
  } else {
    $showArea.hide();
    $input.show();
  }
}

/**
 * chooseLang(): select language for "use" or "drop" operation.
 *
 * Depends on configuration. If "ask", prompt user and wait for its response.
 */
function chooseLang(input, link, option, targetFunc) {
  // Work depending on given configuration option.
  if (option == 'ask') {
    langDialog(
      input, link, help[link.dataset[ccFUNC] + 'Lang'], targetFunc);
    return; // wait for user response
  }
  // Define involved lang then execute targetFunc.
  switch (option) {
    case 'current': chosenLang = curLang; break;
    case 'default': chosenLang = defLang; break;
  }
  targetFunc.call(input);
}

/**
 * createWidget(): create widget associated to input element.
 */
function createWidget(input) {
  var $input = $(input),
      generalProps = [
        //'width', 'height', -> only set in attachWidget, when widget visible
        'borderTopWidth', 'borderRightWidth',
        'borderBottomWidth', 'borderLeftWidth',
        'borderTopStyle', 'borderRightStyle',
        'borderBottomStyle', 'borderLeftStyle',
        'borderTopColor', 'borderRightColor',
        'borderBottomColor', 'borderLeftColor',
        'marginTop', 'marginRight', 'marginBottom', 'marginLeft',
        'paddingTop', 'paddingRight', 'paddingBottom', 'paddingLeft',
      ],
      textProps = [
        'fontFamily', 'fontSize', 'fontWeight',
      ];
    
  if (!$input.hasClass(CKEDITORMOD)) { // (or is managed through CKEditor)
    
    // Poll till input becomes visible, if not yet.
    if (!$input.is(':visible')) {
      setTimeout(createWidget, 100, input);
      return;
    }
    
    // Create a widget instance associated to input.
    var $showInstance = $showTemplate.clone(true, true)
      .css(getCSS($input, generalProps))
      .css(getCSS($input, textProps))
      .insertAfter($input)
      .attr(INPUTID, input.id);
    $(classTEXT, $showInstance).css(getCSS($input, textProps));
    
    // Finally set input depending on current content.
    setInput.call(input);
  }
}

/**
 * detectChanges(): do setInput() following Drupal.behaviours implied changes.
 */
function detectChanges(originatingId) {
  // Execute setInput() on every changed input, except original one
  for (var inputId in changeSurvey) {
    if (inputId != originatingId) {
      var input = document.getElementById(inputId);
      if (input.value != changeSurvey[inputId]) {
        changeSurvey[inputId] = input.value;
        setInput.call(input);
      }
    }
  }
  surveyInProgress = undefined;
}

/**
 * langDialog(): offers all defined languages, with Ok/Cancel.
 */
function langDialog(input, link, content, targetFunc) {
  // Rebuild depending on current context.
  $('.content', $dialog).html(content);
  $cache.add($dialog).show();
  // CAUTION: if offset() occurs before show(), strange behaviour.
  $dialog.offset($(link).offset());
  $('input:first', $dialog).attr({checked:'checked'}).get(0).focus();
  scrollAdjust($dialog[0]);
  
  // Bind events.
  $(':input', $dialog)
  .unbind('.multilang')
  .bind(KEYDOWN, function(event) {
    if (event.which == 27) {
      $('button[value=cancel]',$dialog).click();
    }
    return false;
  });
  $('button', $dialog).bind(CLICK, function() {
    if (this.value == 'ok') {
      // Define involved lang then execute targetFunc.
      chosenLang = $('input[name=' + LANG + ']:checked', $dialog).val();
      targetFunc.call(input);
    }
    $cache.add($dialog).hide();
    return false;
  });
}

/**
 * normalizeText(): rebuild a "multi" segment according to defined languages.
 */
function normalizeText(string) {
  // Reformat each segment found in string to fit with template.
  return string.replace(
    lookForSegment,
    function(segment) {
      // Create a new segment populated from lang blocks found in original
      // segment. Result will contain all currently referenced langs, even
      // when absent in original segment. At the opposite, if unknown lang
      // blocks exist in original segment, they're lost.
      var newSegment = '';
      for (var lang in langs) {
        newSegment +=
          langBlock(langOrDefault(lookForLang(segment, lang), lang), lang);
      }
      return wrapSegment(newSegment);
    }
  );
}

/**
 * renderSegment(): render a "multi" segment in the current language.
 */
function renderSegment(segment, canHighlight) {
  var result = lookForLang(segment, curLang, /*returnFalseIfAbsent*/ true);
  /* CAUTION:
     . "missing translation" means that either:
       - lang block does not exist
       - or lang block exists and contains language native name
     . "empty translation" means that lang block exists and is empty
       (depending on configuration, it may be replaced by default site
       language or left empty)
  */
  if (
    // Missing translation.
    (result === false || result == langNative)
    ||
    // Empty translation, when config states empty viewed as missing.
    (!result && fallback.empty == 'replace')
  ) {
    // Empty result, fallback depending on configuration.
    result = fallback.option == 'default' ?
      lookForLang(segment, defLang) : '[multilang-custom]';
    // Hightlight default, unless denied.
    if (canHighlight) {
      // Merely set flag to be used in renderString().
      result = wrapHighlight(result);
    }
  }
  return result;
}

/**
 * renderString(): render "multi" segments and linebreaks in a given string.
 */
function renderString(string, canHighlight, replacePara) {
  return string
    /* Preserve tag-like data (such as <none> or <front>), because @string
       may come from node.textContent, so:
       - original node's "&lt;tag&gt;" became "<tag>" in current @string
       - then when further $node.replaceWith() it would become a real <tag>
       - at the opposite, using node.data= would turn back to "&lt;tag&gt;",
         but would do the same on the added highlighting <span>
       - so the solution is to preventively turn "<tag>" into "&lt;tag&gt;"
    */
    .replace(/<([^>]*)>/g, '&lt;$1&gt;')
    // Render "multi" segments in the current language.
    .replace(
      lookForSegment, 
      function(segment) {
        return renderSegment(segment, canHighlight);
      }
    )
    // Render customized message if any.
    .replace(lookForCustom, fallback.message)
    // Highlight feltback texts.
    // (also processes [multilang-highlight] from original HTML, if any)
    .replace(
      lookForHighlight, 
      function(match, text) {
        return canHighlight ? wrap(text, fallback.span) : text;
      }
    )
    // Render linebreak flags by break|space depending on configuration.
    .replace(lookForPara, replacePara);
}

/**
 * renderTextNode(): render "multi" segments and linebreaks.
 */
function renderTextNode(node) {
  var $node = $(node);
  /**/ // <patch> to work with jQuery < 1.6
  /**/ var replacePara = NBSP;
  /**/ for (var i in filters.useLinebreak) {
  /**/   if ($node.closest(filters.useLinebreak[i]).length) {
  /**/     replacePara = BR;
  /**/     break;
  /**/   }
  /**/ }
  /**/ // </patch>
  $node.replaceWith(
    renderString(
      node.textContent,
      /*canHighlight*/ !$node.closest('head').length,
      /*replacePara*/
      /**/ //($node.closest(filters.useLinebreak).length ? BR : NBSP)
      /**/ replacePara // above statement needs jQuery >= 1.6
    )
  );
}

/**
 * saveEntry(): update input, clear input area.
 */
function saveEntry() {
  // Populate curInput element with a well-formed segment built from $entryArea.
  var newSegment = '',
      value = '';
  for (var lang in langs) {
    value = $(classENTRY + '-' + lang + ' textarea', $entryArea)[0].value;
    // When element is <input>, turn linebreaks into linebreak flags.
    if (curInput.tagName == 'INPUT') {
      value = value.replace(/\n/g, PARA);
    }
    newSegment += langBlock(value, lang);
  }
  stackIt(curInput);
  curInput.value = wrapSegment(newSegment);
  setInput.call(curInput);
}

/**
 * setInput(): offer a link to use/drop format, attach/detach widget.
 */
function setInput() {
  // "this" is the input or textarea element.
  
  var $label = getInputLabel(this);
  
  // Update surveyChanges, then notify change to Drupal.behaviours.
  changeSurvey[this.id] = this.value;
  $(this).trigger('keyup');
  /* Wait for implied changes to take effect, due to Drupal.behaviours, then
  look for changes on other input's. This way is used because focus is on the
  current input, so the change event will not fire on the other input's: they
  don't have focus and will not loose it. */
  if (!surveyInProgress) {
    // (avoid launching nested survey)
    surveyInProgress = setTimeout(detectChanges, 100, this.id);
  }
  
  // Set links and showArea depending on the current context.
  $('a', $label).remove();
  if (this.value.match(/^\[multi\][\s\S]*\[\/multi\]/ig,'\]\[')) {
    // This is a "multi" segment: offer to go back to "single", attach widget.
    setLink('drop', this, $label);
    attachWidget(this, true);
    $(classLINK, $label).bind(CLICK, clickDrop);
  } else {
    // This is raw text: offer to "multify".
    setLink('use', this, $label);
    attachWidget(this, false);
    $(classLINK, $label).bind(CLICK, clickUse);
  }
  
  // Add undo link if available history.
  if (stack[this.id] && stack[this.id].length) {
    $label.append(
      wrapTag(
        //'[' +help.undoText + ']',
        '<img src="' + path + '/undo.png" alt="' + help.undoText + '" />',
        '<a href="javascript:void(0);" class="' + UNDO + '"'
        + ' ' + INPUTID + '="' + this.id + '"'
        + ' title="' + help.undoTitle + '">'
      )
    );
    $(classUNDO, $label).bind(CLICK, clickUndo);
  }
}

/**
 * setLink(): offer "Use" or "Drop" link.
 */
function setLink(func, input, $label) {
  // Add new link like
  $label.append(
    wrapTag(
      help[func + 'Text'],
      '<a href="javascript:void(0);" class="' + LINK + '"'
      + ' ' + INPUTID + '="' + input.id + '"'
      + ' ' + FUNC + '="' + func + '"'
      + ' title="' + help[func + 'Title'] + '">'
    )
  );
}

/**
 * startEntry(): open entryArea.
 */
function startEntry(input, selectLang) {
  // Register current input.
  curInput = input;
  // Distribute text into dedicated cells.
  for (var lang in langs) {
    $(classENTRY + '-' + lang + ' textarea', $entryArea)[0].value =
      // Get text in the involved language.
      lookForLang(input.value, lang)
      // Turn linebreak flags into real linebreaks.
      .replace(PARA, '\n', 'g');
  }
  // Position entryArea to override original input.
  var $input = $(input),
      $showArea = $input.next();
  $entryArea
    // CAUTION: show before setting offset, or ADDS top/left values!
    .show()
    // CAUTION: use showArea as template (input offset is wrong, since hidden)
    .offset($showArea.offset())
    .css({width:
      // This method allows using jQuery < 1.8
      $showArea.outerWidth()
      - parseInt($showArea.css('borderLeftWidth') | 0)
      - parseInt($showArea.css('borderRightWidth') | 0)
    });
  $('textarea', $entryArea).css({
    fontFamily: $input.css('fontFamily'),
    fontSize: $input.css('fontSize'),
    fontWeight: $input.css('fontWeight'),
    });
  $(classENTRY + '-' + selectLang + ' textarea', $entryArea).focus();
  scrollAdjust($entryArea[0]);
  return false;
}

/**
 * stopEntry(): cancel current entry operation.
 */
function stopEntry() {
  // Cancel $entryArea operation (recent changes are lost)
  $entryArea.hide();
  curInput = undefined;
}
//----------------------------------------------------------------------- EVENTS
/**
 * blurEntry(): just leaved focus on entryArea.
 */
function blurEntry(event) {
  // Do nothing if just {Esc}aped.
  if (curInput) {
    // May be simply navigating inside of $entryArea, let time enough to focus.
    setTimeout(function() {
      if ($('textarea:focus', $entryArea).length == 0) {
        // Really gone elsewhere, update original input.
        saveEntry();
        stopEntry();
      }
    }, 10);
  }
}

/**
 * clickDrop(): clicked "Drop Multilang format" link.
 */
function clickDrop() {
  // "this" is the link element
  stopEntry();
  var input = referredInput(this);
  
  // Preserve text of the configuration-defined language.
  chooseLang(input, this, multify.backLang, function() {
    // Here "this" is the input element.
    stackIt(this);
    this.value = lookForLang(this.value, chosenLang);
    if (this.value == wrap(langs[chosenLang], '...')) {
      // missing translation (contains only langNative)
      this.value = '';
    }
    setInput.call(this);
  });
  return false;
}

/**
 * clickShow(): open input area, selecting the hovered language label.
 */
function clickShow(event) {
  var input = referredInput(this);
  startEntry(
    input,
    $(event.target).hasClass(LANG) ?
      event.target.innerHTML : input.dataset[ccSELECTLANG]
  );
}

/**
 * clickUndo(): restore previous input value.
 */
function clickUndo() {
  var input = referredInput(this);
  unstackIt(input);
  setInput.call(input);
  return false;
}

/**
 * clickUse(): clicked "Use Multilang template" link.
 */
function clickUse() {
  // "this" is the link element
  stopEntry();
  var input = referredInput(this);
  
  // If empty input, first turn it into an empty segment.
  if (!input.value) {
    input.value = wrapSegment('');
  }
  
  if (input.value.match(lookForSegment)) {
    // Input is already a segment: merely normalize it.
    stackIt(input);
    input.value = normalizeText(input.value);
    setInput.call(input);
  } else {
    // Input is raw text: turn it into a configuration-depending language block.
    chooseLang(input, this, multify.fromLang, function() {
    // Here "this" is the input element.
      stackIt(this);
      this.value =
        normalizeText(wrapSegment('[' + chosenLang + ']' + this.value));
      setInput.call(this);
    });
  }
  return false;
}

/**
 * keydownEntry(): manage {Ctrl-Enter}/{Esc}/{Tab}/{Shift-Tab} in entryArea.
 */
function keydownEntry(event) {
  switch (event.which) {
    case CTRL:
    case SHIFT:
      lastWhich = event.which;
      break;
    case TAB:
      // TAB stroken: keep looping inside of entryArea.
      var entryFirst = $(classENTRYFIRST, $entryArea)[0],
          entryLast = $(classENTRYLAST, $entryArea)[0],
          tabUp = (lastWhich == SHIFT);
      lastWhich = undefined;
      if (event.target == entryLast && !tabUp) {
        entryFirst.focus();
        return false;
      }
      if (event.target == entryFirst && tabUp) {
        entryLast.focus();
        return false;
      }
      break;
    case ESC:
      // ESC stroken: cancel operation.
      stopEntry();
      lastWhich = undefined;
      return false;
    case  ENTER:
      if (lastWhich == CTRL) {
        // CTRL-ENTER stroken: validate operation.
        saveEntry();
        stopEntry();
        lastWhich = undefined;
        return false;
      }
      lastWhich = undefined;
      break;
    default:
      lastWhich = undefined;
  }
}

/**
 * mouseenterShow(): display text in the hovered language label.
 */
function mouseenterShow() {
  var input = referredInput(this.parentElement),
      selectLang = this.innerHTML;
  $(this)
    .addClass(SELLANG)
    .siblings().removeClass(SELLANG)
    .filter(classTEXT).html(lookForLang(input.value, selectLang));
  input.dataset[ccSELECTLANG] = selectLang;
}

/**
 * mousedownGrippie(): starts showArea to follow textarea height.
 */
function mousedownGrippie() {
  // Register source/target to improve performance while moving.
  resizing.$showArea = $(this).siblings(classSHOW);
  resizing.$input = $(referredInput(resizing.$showArea[0]));
  
  $(document)
  .bind(MOUSEUP, function() {
    $(document).unbind(MOUSEMOVE + ' ' + MOUSEUP);
  })
  .bind(MOUSEMOVE, function() {
    resizing.$showArea.outerHeight(resizing.$input.outerHeight());
  });
}
// ------------------------------------------------------------- ANCILIARY FUNCS
function buildConstants(list, which) {
  var item, magicName, statement;
  for (var i in list) {
    item = list[i];
    magicName = item.toUpperCase();
    switch(which) {
      case 'names':
        statement =
        // NAME = "multilang-name", classNAME = "." + NAME
          magicName + '="' + MULTILANG + '-' + item + '",'
          + 'class' + magicName + '="."+' + magicName;
        break;
      case 'datanames':
        statement =
        // DATANAME = "data-multilang-dataname", ccDATANAME = multilangDataname
          magicName + '="data-' + MULTILANG + '-' + item + '",'
          + 'cc' + magicName + '="' + MULTILANG.toLowerCase()
            + item[0].toUpperCase()+item.substr(1) + '"';
        break;
      case 'events':
      // EVENT = "event.multilang"
        statement = magicName + '="' + item + '.' + MULTILANG + '"';
        break;
    }
    evals.push(statement);
  }
}
function getCSS($element, properties) {
  // Return an object of key/value pairs of @properties for @$element.
  var result = {};
  for (var i in properties) {
    result[properties[i]] = $element.css(properties[i]);
  }
  return result;
}
function getInputLabel(input) {
  // Return $label associated to @input, or false if not found.
  var $label = 
    $(input.tagName == 'INPUT' ? input : input.parentElement)
    .siblings('label[for=' + input.id + ']');
  return $label.length ==1 ? $label : false;
}
function langBlock(string, lang) {
  // Return "[@lang]@string"
  return '[' + lang + ']' + string;
}
function langOrDefault(string, lang) {
  // Return @string defaulting to "...@lang-native..." if empty
  return !!string ? string : wrap(langs[lang], '...');
}
function lookForLang(segment, lang, returnFalseIfAbsent) {
  // Look at @segment to return a @lang block content (possibly empty).
  // CAUTION: currently, texts are assumed not to contain any "[".
  //var block = segment.match(new RegExp('\\[' + lang + '\\]([^\\[]*)','i'));
  var block = segment.match(
    new RegExp('\\[' + lang + '\\]([[\\s\\S]*?)' + langMarkOrEnd,'i')
  );
  // If lang code absent, return false if returnFalseIfAbsent, empty otherwise.
  returnFalseIfAbsent |= false;
  return !!block ? block[1] : (returnFalseIfAbsent ? false : '');
}
function putEmbeddedVars(text, args) {
  // Equivalent of Drupal.formatString, for directly translated help.
  args = args || {};
  for (var argName in args) {
    text = text.replace(new RegExp('@' + argName), args[argName]);
  }
  return text;
}
function referredInput(element) {
  // Return the input element that data-multilang-input-id refers to.
  return document.getElementById(element.dataset[ccINPUTID]);
}
function scrollAdjust(element) {
  // Don't use .scrollIntoView(): when set top=0, admin-menu overwrites element.
  var elemRect = element.getBoundingClientRect(),
      winHeight = $(window).height();
  if(elemRect.bottom > winHeight) {
    window.scrollBy(0, elemRect.bottom - winHeight);
  }
}
function stackIt(input) {
  // Save current input value into history, if changed.
  if (!stack[input.id]) {
    stack[input.id] = [];
  }
  if (!stack[input.id].length || stack[input.id][0] != input.value) {
    stack[input.id].unshift(input.value);
  }
}
function unstackIt(input) {
  // Restore input to its most recently saved value.
  input.value = stack[input.id].shift();
}
function wrap(text, before, after) {
  // Wrap @text between @before and @after, where @after defaults to @before.
  return before + text + (!!after ? after : before);
}
function wrapHighlight(text) {
  return wrap(text, '[multilang-highlight]', '[/multilang-highlight]');
}
function wrapSegment(text) {
  return wrap(text, '[multi]', '[/multi]');
}
function wrapTag(text, tagOpen) {
  return wrap(text, tagOpen, tagOpen.replace(/^<([^ >]*)[\s\S]*/i,'</$1>'));
}

})(jQuery);
//------------------------------------------------------------------------------
// .offset(...) on hidden element -> https://jsfiddle.net/06krxh3o/1/