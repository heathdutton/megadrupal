(function($) {
  var l = Drupal.settings.Instagram.langs
    , data = Drupal.settings.Instagram.data
  CKEDITOR.plugins.add('instagram', {
    init: function(editor) {
      var items = [];
      Object.keys(data).forEach(function(i) {
        items.push([data[i].title, i]);
      });
      var first = 0 in items ? items[0][1] : false;
      var change = function(i, changed) {
        changed = changed || false;
        var pos = 'none';
        if (Drupal.settings.Instagram.temp) {
          i = Drupal.settings.Instagram.temp.id;
          pos = Drupal.settings.Instagram.temp.pos;
          Drupal.settings.Instagram.temp = null;
          $('.instagram-feed select').val(i);
        }
        if (i in data) {
          i = data[i];
          $('.instagram-type input').val(l['type' + i.type]);
          $('.instagram-layout input').val(Drupal.formatPlural(i.cols, l.col, l.cols) +
                  ' X ' + Drupal.formatPlural(i.rows, l.row, l.rows));
          $('.instagram-size input').val(i.size + ' X ' + i.size);
        }
        changed || $('.instagram-pos select').val(pos);
      };
      Drupal.settings.Instagram.temp = null;

      editor.addCommand('instagram', new CKEDITOR.dialogCommand('instagram_dialog'));

      editor.ui.addButton('instagram_button', {
        label: l.title,
        command: 'instagram',
        icon: this.path + 'images/instagram.png'
      });

      CKEDITOR.dialog.add('instagram_dialog', function(editor) {
        return {
          title: l.title,
          minWidth: 360,
          minHeight: 180,
          contents: [{
            elements: [{
              type: 'select',
              id: 'feed',
              className: 'instagram-feed',
              label: l.feed,
              labelLayout: 'horizontal',
              items: items,
              'default': first,
              onShow: function() {
                change(this.getValue());
              },
              onChange: function() {
                change(this.getValue(), true);
              },
              commit: function(data) {
                data.feed = this.getValue();
              }
            }, {
              type: 'text',
              id: 'type',
              className: 'instagram-type',
              label: l.type,
              onLoad: function() {
                this.disable();
              }
            }, {
              type: 'text',
              id: 'layout',
              className: 'instagram-layout',
              label: l.layout,
              onLoad: function() {
                this.disable();
              }
            }, {
              type: 'text',
              id: 'size',
              className: 'instagram-size',
              label: l.size,
              onLoad: function() {
                this.disable();
              }
            }, {
              type: 'select',
              id: 'pos',
              className: 'instagram-pos',
              label: l.pos,
              labelLayout: 'horizontal',
              items: [ [ l.none, 'none' ], [ l.left, 'left' ], [ l.center, 'center' ], [ l.right, 'right' ] ],
              'default': 'none',
              commit: function(data) {
                data.pos = this.getValue();
              }
            }]
          }],
          buttons: [{
              type: 'button',
              id: 'create',
              label: l.create,
              title: l.create,
              onClick: function() {
                window.open('/node/add/instagram-feed');
                CKEDITOR.dialog.getCurrent().hide();
              }
            },
            CKEDITOR.dialog.cancelButton,
            CKEDITOR.dialog.okButton,
          ],
          onOk: function() {
            var data = {};
            this.commitContent(data);
            editor.insertHtml('[api:instagram[' + data.feed + ',"' + data.pos + '"]/]');
          }
        }
      });

      editor.on('doubleclick', function(e) {
        var element = e.data.element;
        if (element.is('img') && 'instagram-fake' === element.getAttribute('data-cke-real-element-type')) {
          var data = decodeURIComponent(element.getAttribute('data-cke-realelement'))
            .match(/\[api:instagram\[([0-9]+),"(none|left|center|right)"\]\/\]/);
          Drupal.settings.Instagram.temp = { id: data[1], pos: data[2] };
          e.data.dialog = 'instagram_dialog';
        }
      });

      editor.addCss('img.instagram-fake{background: url("' + this.path +
              '/images/instagram-placeholder.png") no-repeat center center/100% 100%}');
      editor.addCss('img.instagram-fake.broken{background: url("' + this.path +
              '/images/instagram-placeholder-broken.png") no-repeat center center/100% 100%;height:150px;width:200px}');
      editor.addCss('img.instagram-fake.pos-none{margin:.5em}');
      editor.addCss('img.instagram-fake.pos-left{float:left;margin:1em 1em 1em 0}');
      editor.addCss('img.instagram-fake.pos-center{clear:both;display:block;margin:.5em auto}');
      editor.addCss('img.instagram-fake.pos-right{float:right;margin:1em 0 1em 1em}');
      Object.keys(data).forEach(function(i) {
        var size = + data[i].size + 2 * data[i].border;
        editor.addCss('img.instagram-fake.id' + i + '{height:' + size * data[i].rows +
                'px;width:' + size * data[i].cols + 'px}');
      });
    },

    afterInit: function(editor) {
      var dataProcessor = editor.dataProcessor
        , dataFilter = dataProcessor && dataProcessor.dataFilter;
      if (dataFilter) {
        dataFilter.addRules({
          text: function(text) {
            return text.replace(/\[api:instagram\[[0-9]+,"(none|left|center|right)"\]\/\]/g, function(match) {
              var id = match.match(/[0-9]+/)[0]
                , pos = 'pos-' + match.match(/(none|left|center|right)/)[0]
                , broken = id in data ? '' : ' broken'
                , fragment = new CKEDITOR.htmlParser.fragment.fromHtml(match)
                , element = fragment && fragment.children[0];
              element.attributes = {};
              var fake = editor.createFakeParserElement(element, 'instagram-fake id' +
                      id + ' ' + pos + broken, 'instagram-fake', false);
              fake.attributes.alt = fake.attributes.title = broken ? l.broken : l.fake;
              var writer = new CKEDITOR.htmlParser.basicWriter();
              fake.writeHtml(writer);
              return writer.getHtml();
            });
          }
        });
      }
    }
  });
})(jQuery);
