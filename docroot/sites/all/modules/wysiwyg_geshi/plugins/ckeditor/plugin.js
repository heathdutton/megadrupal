CKEDITOR.plugins.add('geshi', {
  requires: 'dialog',
	init: function(editor) {

    // If we don't have out settings in place it means that the GeSHi Filter
    // is not configured correctly so there's nothing we can do.
    if (!Drupal.settings.wysiwyg_geshi) {
      return false;
    }

    // Grab the settings added by the module and convert them to the format
    // expected by CKEditor.
    var geshi_lang_obj = Drupal.settings.wysiwyg_geshi.languages,
        geshi_lang_array = [],
        geshi_tag_open,
        geshi_tag_close,
        geshi_regex;

    for (var i in geshi_lang_obj) {
      var temp = [];
      temp.push(geshi_lang_obj[i], i);
      geshi_lang_array.push(temp);
    }

    // Regex used to extract tags when building the dialog field values.
    geshi_tag_open = Drupal.settings.wysiwyg_geshi.tag_open;
    geshi_tag_close = Drupal.settings.wysiwyg_geshi.tag_close;
    geshi_regex = new RegExp('\\' + geshi_tag_open + '[a-z0-9]+' + '\\' + geshi_tag_close);

    editor.addCommand('geshiDialog', new CKEDITOR.dialogCommand('geshiDialog'));

    editor.ui.addButton && editor.ui.addButton( 'geshi',
      {
        label : Drupal.t('Insert GeSHi code snippet'),
        icon : this.path + '../../wysiwyg_geshi.png',
        command: 'geshiDialog',
        toolbar: 'insert,99'
      });

    CKEDITOR.dialog.add( 'geshiDialog', function( editor )
    {
      return {
        title : Drupal.t('Insert GeSHi code snippet'),
        minWidth : 540,
        minHeight : 380,
        contents : [
          {
            id : 'general',
            label : Drupal.t('GeSHi code snippet'),
            elements : [
              {
                type : 'select',
                id : 'highlight',
                label : Drupal.t('Language'),
                required : true,
                items : geshi_lang_array,
                'default': geshi_lang_array[0][1],
                setup : function(element) {
                  var html = element.getHtml();

                  if (html) {
                    var tag = geshi_regex.exec(html);

                    if (tag) {
                      tag = tag[0].replace(geshi_tag_open, '').replace(geshi_tag_close, '');
                      this.setValue(tag);
                    }
                  }
                }
              },
              {
                type : 'textarea',
                id : 'contents',
                label : Drupal.t('Code snippet'),
                cols: 140,
                rows: 22,
                required : true,
                setup : function(element) {
                  var html = element.getHtml();
                  if (html) {
                    var tag = geshi_regex.exec(html);

                    if (tag) {
                      tag = tag[0];
                      var closing_tag = tag.slice(0, 1) + "/" + tag.slice(1);
                      html = jQuery.trim(html.replace(tag, '').replace(closing_tag, ''));
                    }

                    var div = document.createElement('div');
                    div.innerHTML = html;
                    this.setValue(div.firstChild.nodeValue);
                  }
                },
                commit : function(element) {
                  element.setHtml(this.getValue());
                }
              }
            ]
          }
        ],
        onShow : function() {
          var sel = editor.getSelection(),
            element = sel.getStartElement();

          if (element) {
            element = element.getAscendant('pre', true );
          }

          if (!element || element.getName() != 'pre') {
            element = editor.document.createElement('pre');
          }

          this.pre = element;
          this.setupContent(this.pre);
        },
        onOk : function() {
          var contents = this.getValueOf('general', 'contents').toString(),
              highlight = this.getValueOf('general', 'highlight').toString();

          var element = CKEDITOR.dom.element.createFromHtml(
          '<pre>' + geshi_tag_open + highlight + geshi_tag_close +'\n' + contents + '\n' + geshi_tag_open + '/' + highlight + geshi_tag_close + '</pre>'
          );

          this.pre.remove();
          editor.insertElement(element);
        }
      };
    });
	}
});
