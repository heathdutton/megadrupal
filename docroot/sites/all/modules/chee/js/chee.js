/**
 * @file
 * JS actions.
 */

(function($) {
  var chee = {
    selector: 'code',
    tabReplace: '  ',
    lineBreaks: false,
    frontTheme: '',
    themePath: '',
    // Delay, before load scripts, after switch to preview mode.
    hPrevieTime: 500,
    // Depay, before load scripts, after keyup on split edit mode.
    hEditTime: 250,

    init: function(settings) {
      this.tabReplace = settings.tabReplace;
      this.lineBreaks = settings.lineBreaks;
      this.frontTheme = settings.frontTheme;
      this.frontThemePath = "/" + settings.libraryPath + "/styles/" + settings.frontTheme + ".css";
      this.backThemePath = "/" + settings.libraryPath + "/styles/" + settings.backTheme + ".css";

      if (hljs) {
        this.hFrontCode(this.frontThemePath);
        this.hBackCode(this.backThemePath);
      }
      else {
        return false;
      }
    },
    makeCssLine: function(themePath) {
      return '<link rel="stylesheet" id="chee" href="' + themePath + '" type="text/css" media="screen">';
    },
    wrapElement: function(el) {
      if ($(el).parent('pre') != 1) {
        $(el).wrap('<pre/>');
      }
    },
    // Highlight Code on Front end.
    hFrontCode: function(themePath) {
      $(this.selector).each(function(index, element) {
        hljs.highlightBlock(element, chee.tabReplace, chee.lineBreaks);
        chee.wrapElement(element);
        $(element).parent('pre').addClass(chee.frontTheme);
      });
      $('html head').append(chee.makeCssLine(themePath));
    },
    // Highlight Code on Back end.
    hBackCode: function(themePath) {

      setTimeout(function() {
        $.each($('.page-node-edit iframe'), function(i, parentIFrame) {
          var parentIFrame = $(parentIFrame).contents()
            , viewIFrame = parentIFrame.find('#epiceditor-previewer-frame')
            , viewIFrameContent = viewIFrame.contents()
            , editIFrame = parentIFrame.find('#epiceditor-editor-frame')
            , editIFrameContent = editIFrame.contents()
            , controls = parentIFrame.find('#epiceditor-utilbar');

          viewIFrameContent.find('head').eq(0).append(
            chee.makeCssLine(themePath)
          );

          controls.click(function() {
            viewIFrameContent.find('pre code').each(function(index, element) {
              hljs.highlightBlock(element, chee.tabReplace);
            });
          });

          editIFrameContent.find('body').keyup(function() {
            if (viewIFrame.css('display') == 'block' && editIFrame.css('display') == 'block') {
              setTimeout(function() {
                viewIFrameContent.find('pre code').each(function(index, element) {
                  hljs.highlightBlock(element, chee.tabReplace);
                });
              }, chee.hEditTime);
            }
          });
        });
      }, this.hPrevieTime);
    }
  };

  Drupal.behaviors.chee = {
    attach: function(context, settings) {
      if (typeof(hljs) != 'undefined') {
        chee.init(settings.chee);
      }
    }
  }
})(jQuery);
