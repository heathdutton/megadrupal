/**
 * @file
 * MMS plugin for CKEditor.
 */

CKEDITOR.plugins.add('mms',{
  requires: 'widget',
  icons: 'mms',
  init: function(editor) {
    /*
    Test helper
    ------------*/
    showWidget = function(n) {
      return jQuery(
        '.mms',jQuery('iframe')[!!n ? n : 1].contentDocument
      )[0];
    }
    /*
    Private functions
    ------------------*/
    function divOpen(lang/* or "?" */) {
      return '<div class="mms' +
        (!!lang ?
          (
          '-lang" lang="' +
          // Get or set {lang}
          (lang == '?' ? '([^"]+)' : lang) +
          '" title="' +
          (lang == '?' ? '[^"]+' : langs[lang]) +
          '"'
          )
          :
          '"'
        ) +
        '>';
    }
    function divClose(lang/* or "?" */) {
      return '</div>' +
        (!!lang ?
          (
          (lang == '?' ? '\\s*' : '') +
          '<p class="mms">' +
          (lang == '?' ? '[^<]+' : langs[lang]) +
          '</p>'
          )
          :
          ''
        );
    }
    function langTemplate(lang) {
      return divOpen(lang) + '...' + langs[lang] + '...' + divClose(lang);
    }
    function regLang(lang) {
      return new RegExp(
        /* looks for current [lang], ending with any [lang] or nothing
           (each [lang] may be preceded by <p>) */
        pBeg + '\\[' + lang + '\\]\\s*' + contentCapture + pBeg + anyLang, 'i'
      );
    }
    function val(x) {
      // Return x, or an empty string if undefined.
      return (!!x ? x : '');
    }
    /*
    Prepare lang-dependent data
    ----------------------------*/
    var langs = Drupal.settings.mms.langs,
        langList = [],
        langTemplates = [],
        langEditables = {};
    for (var lang in langs) {
      langList.push(lang);
      langTemplates.push(langTemplate(lang));
      langEditables[lang + 'Content'] = {
        selector: '.mms-lang[lang=' + lang + ']',
        allowedContent:
          'h2 h3 h4 h5 h6 hr br div p blockquote ul ol li dl dt dd ' +
          'table col colgroup tbody thead tfoot tr th td caption ' +
          'span iframe img a (*) [*] {*}; ' +
          'strong em u s sub sup code',
      };
    }
    /*
    Prepare RegExp's
    -----------------*/
    var
      // Regexp components.
      pBeg = '\\s*(<p>)?\\s*',
      pEnd = '\\s*(</p>)?\\s*',
      divBeg = divOpen(),
      divEnd = divClose(),
      contentCapture = '([\\s\\S]*?)',

      // Regexp to transcribe "multi" tags into widgets.
      regMulti =
      // Looks for <multi>, or </multi>.
        new RegExp(pBeg + '(?:<|\\[|&lt;)(/?)multi(?:>|\\]|&gt;)' + pEnd, 'ig'),

      // Regexp to isolate widgets content.
      regWidget =
      // Looks for ... in <div class="mms">...</div>.
        new RegExp(divBeg + contentCapture + divEnd, 'ig'),

      // Part of regexp to isolate lang parts.
      anyLang =
      // Looks for [lang] or end.
        '(\\[(?:' + langList.join('|') + ')\\]|$)',

      // Regexp to turn back lang parts into "multi" syntax.
      regLangBack =
        new RegExp(divOpen('?') + contentCapture + divClose('?'), 'ig'),

      // Regexp to turn back widgets into "multi" syntax.
      regMultiBack =
        new RegExp('(' + divBeg + '|' + divEnd + ')', 'ig');

    /*
    Manage data from source input
    ------------------------------*/
    editor.on('setData', function(event) {
      /*
      When it comes from direct user entry, source input may frequently look
      like:
        "[multi]<p>...</p>...<p>...</p>[/multi]"
        (in the case of several paragraphs in the same "multi" block)
      or simply:
        "[multi]<p>...</p>[/multi]"
        (when there is only 1 paragraph).
      In both cases, before firing setData event, ckeditor (HTML purifier)
      modifies data to enforce the HTML5 rule about flow element which must not
      contain both text and child node(s), so the data at this level became:
        "<p>[multi]...</p>...<p>...[/multi]</p>"
      or
        "<p>[multi]...[/multi]</p>".
      To avoid resulting in invalid tags interweaving, the processes below look
      for <p>|</p> tags wrapping [multi]|[/multi] (step 1) and <p> tag preceding
      [{lang}] (step 2), in order to report these tags in the right place while
      transforming data.
       */
      if (!!event.data.dataValue) {
        /*
        1. transcribe any kind of "multi" tags into widgets */
        var data = event.data.dataValue.replace(
          // [multi] or <multi> or &lt;multi&gt; --> <div class="mms">
          regMulti,
          function(match, pOpen, isClosing, pClose) {
            return (
              // Report appended </p> before closing.
              (!!pClose ? '</p>' : '') +
              // "multi" tag --> <div>.
              (!isClosing ? divBeg : divEnd) +
              // Report prepended <p> after opening.
              (!!pOpen ? '<p>' : '')
            );
          }
        );
        /*
        2. manage lang parts inside of widgets */
        event.data.dataValue = data.replace(
          // Isolate "multi" widgets.
          // Get content of <div class="mms">
          regWidget,
          function(match, segment) {
            var widgetContent = '';
            for (var lang in langs) {
              var blockMatch = segment.match(regLang(lang));
              // (blockMatch: [match, pOpen, content, pOpenNext, langNext])
              widgetContent +=
                !!blockMatch ? // Found lang.
                  (
                  // [{lang}] --> <div class="mms-lang" lang="{lang}">
                  divOpen(lang) +
                  (!!blockMatch[1] ? '<p>' : '') + val(blockMatch[2]) +
                  // (if a prepending <p> existed, report it inside content)
                  divClose(lang)
                  )
                : // Missing lang.
                  langTemplate(lang);
            }
            return divBeg + widgetContent + divEnd;
          }
        );
        /*
        3. special feature for ui_translation: encode remaining linebreaks.
        They will be decoded back when getData (see below).
        This is to strictly preserve the structure of text parts outside of
        "multi" segments, when they are copied from source code. */
        if (Drupal.settings.mms.isUiTranslation) {
          event.data.dataValue = event.data.dataValue.replace(
            /\n/g, '&lt;LF&gt;'
          );
        }
      }
    });
    /*
    Manage data from widgets
    -------------------------*/
    editor.on('getData', function(event) {
      if (!!event.data.dataValue) {
        /*
        1. decode linebreaks (see setData above) */
        if (Drupal.settings.mms.isUiTranslation) {
          event.data.dataValue = event.data.dataValue.replace(
            /&lt;LF&gt;/g, '\n'
          );
        }
        /*
        2. affect [lang] labels to lang parts */
        var data = event.data.dataValue.replace(
          // <div class="mms-lang" lang="{lang}"> --> [{lang}]:
          regLangBack,
          function(match, lang, content) {
            return '\[' + lang + '\]' + content;
          }
        );
        /*
        3. transcribe widgets into [multi] tags (native form only) */
        event.data.dataValue = data.replace(
          // <div class="mms"> --> [multi]
          regMultiBack,
          function(match) {
            return '\n\[' + (match == divEnd ? '/' : '') + 'multi\]';
          }
        );
      }
    });
    /*
    Define widget
    --------------*/
    editor.widgets.add('mms', {
      upcast: function(element) {
        return element.name == 'div' && element.hasClass('mms');
      },
      allowedContent:
        'div(!mms); div(!mms-lang)[lang,title]; p(mms)',
      requiredContent: 'div(!mms)',
      template: divBeg + langTemplates.join('\n') + divEnd,
      editables: langEditables,
    });
    editor.ui.addButton('mms',{
      label:    'MMS',
      icon:     this.path + 'mms.png',
      command:  'mms',
    });
    editor.addContentsCss(this.path + 'mms.css');
  }
});
