/**
 * @file
 * Asset plugin for CKEditor.
 */
var Assets;
(function ($) {
  // If for some reason buttons should not be accessible - remove them from rendering.
  if (Drupal.settings.ckeditor
    && Drupal.settings.ckeditor.plugins
    && Drupal.settings.ckeditor.plugins.asset
    && Drupal.settings.ckeditor.plugins.asset.removeButtons) {
    var removeButtons = Drupal.settings.ckeditor.plugins.asset.removeButtons.join(',');
    CKEDITOR.config.removeButtons = CKEDITOR.config.removeButtons
      ? CKEDITOR.config.removeButtons + ',' + removeButtons : removeButtons;
  }

  // Temporary container for asset html.
  var tempContainer = document.createElement('DIV'),
    tagCache = {},
    cutted = null;

  // Assets object.
  Assets = {
    selectedElement: null,

    getCKeditorVersion: function () {
      if (CKEDITOR.version) {
        var explodedVersion = CKEDITOR.version.split('.');
        return explodedVersion[0] ? parseInt(explodedVersion[0]) : null;
      }
    },

    getTranslatedString: function (editor, stringKey) {
      return (Assets.getCKeditorVersion() >= 4) ? editor.lang.asset[stringKey] : editor.lang[stringKey];
    },

    insertAssetInEditor: function (editor, html) {
      // If we are dropping directly on other selected Asset insert right after it.
      if (Assets.selectedElement) {
        var selection = editor.getSelection();
        var selectedElement = Assets.selectedElement;
        Assets.deselect();
        var newline = Assets.getEnterElement(editor);
        newline.insertAfter(selectedElement);
        selection.selectElement(newline);
      }

      // Because incoming html could be not well-formed html, but also contain text nodes and comments,
      // we could not use simple insertElement or insertHtml here.
      var tmp = new CKEDITOR.dom.element('div'), children, skip, item;
      tmp.setHtml(html);
      children = tmp.getChildren();
      skip = 0;
      while (children.count() > skip) {
        item = children.getItem(skip);
        switch(item.type) {
          case Node.ELEMENT_NODE:
            editor.insertElement(item);
            break;
          case Node.TEXT_NODE:
            editor.insertText(item.getText());
            skip++;
            break;
          case Node.COMMENT_NODE:
            editor.insertHtml(item.getOuterHtml());
            skip++;
            break;
        }
      }
    },

    removeAssetFromEditor: function (editor, element, cut) {
      if (element) {
        if (cut) {
          cutted = element;
        }
        Assets.deselect();
        var range = editor.getSelection().getRanges()[ 0 ];
        if (!range) {
          range = new CKEDITOR.dom.range(editor.document);
        }
        range.moveToPosition(element, CKEDITOR.POSITION_BEFORE_START);
        element.remove();
        range.select();
      }
    },

    select: function (element) {
      this.deselect();
      this.selectedElement = element;
      this.selectedElement.addClass('selected');
    },

    deselect: function () {
      var element = null, removeSelectedClass = function (el) {
        var cl, i, cl_arr;
        if (el) {
          if (el.removeClass) {
            el.removeClass('selected');
          }
          else {
            if (el.attributes && el.attributes['class']) {
              cl = el.attributes['class'];
              cl_arr = cl.split(' ');
              cl = [];

              for (i = 0; i < cl_arr.length; i = i + 1) {
                if (cl_arr[i] !== 'selected') {
                  cl.push(cl_arr[i]);
                }
              }

              el.attributes['class'] = cl.join(' ');
            }
          }
        }
        return el;
      };

      if (arguments.length && arguments[0]) {
        element = removeSelectedClass(arguments[0]);
      }

      if (this.selectedElement) {
        removeSelectedClass(this.selectedElement);
        this.selectedElement = null;
      }

      return element;
    },

    getSelected: function (editor) {
      if (this.selectedElement) {
        return this.selectedElement;
      }

      var range = editor.getSelection().getRanges()[0];
      if (!range) {
        return false;
      }
      range.shrink(CKEDITOR.SHRINK_TEXT);

      if (range.startContainer) {
        var node = range.startContainer;

        while (node && !(node.type === CKEDITOR.NODE_ELEMENT && node.data('asset-cid'))) {
          node = node.getParent();
        }

        return node;
      }
    },

    parseId: function (tag_id) {
      var arr = tag_id.split(':'), obj = {'aid': arr[0], 'type': arr[1], 'hash': arr[2]};
      return arguments.length > 1 ? obj[arguments[1]] : obj;
    },

    generateId: function (tag_id) {
      var tagObj = this.parseId(tag_id), time = new Date().getTime();
      return [tagObj.aid, tagObj.type, time].join(':');
    },

    getTagData: function (tag) {
      var params = {};
      var matches = tag.match(/\[\[asset:([_a-zA-Z0-9]+):([0-9]+)\s\{((\n|.)*?)\}\]\]/);

      if (matches) {
        var paramsString = matches[3];
        paramsString = '{' + paramsString + '}';

        try {
          params = JSON.parse(paramsString);
          params.aid = matches[2];
          params.type = matches[1];

          if (!params.mode) {
            params.mode = 'full';
          }

          if (!params.align || (params.mode == 'full')) {
            params.align = 'none';
          }
        }
        catch (err) {
          // Empty error handler.
        }
      }

      return params;
    },

    dialog: function (editor, type) {
      return function () {
        return {
          title: Assets.getTranslatedString(editor, 'assets_title'),
          minWidth: 800,
          minHeight: 600,
          contents: [
            {
              id: 'asset_frame',
              label: Assets.getTranslatedString(editor, 'assets_label'),
              expand: true,
              elements: [
                {
                  type: 'iframe',
                  src: Drupal.settings.basePath + Drupal.settings.pathPrefix + 'admin/assets/add/' + type + '/?render=popup',
                  width: '100%',
                  height: '100%'
                }
              ]
            }
          ],
          buttons: [CKEDITOR.dialog.cancelButton]
        };
      };
    },

    adjustDialogHeight: function () {
      // CKeditor 4 have bug into plugins/dialog/plugin.js line 1036.
      // Developers forgot to add height:100% into iframe wrapper.
      // In CKeditor 3 this code present.
      setTimeout(function () {
        $('.cke_dialog_contents .cke_dialog_ui_vbox.cke_dialog_page_contents').css('height', '100%');
      }, 0);
    },

    openDialog: function (editor, dialogName, src, element) {
      editor.openDialog(dialogName, function () {
        this.definition.contents[0].elements[0].src = src;

        if (typeof(element) !== 'undefined') {
          this._outdatedAssetEl = element;
        }

        // Fix height iframe wrapper issue with ckeditor 4.
        Assets.adjustDialogHeight();
      });
    },

    searchDialog: function (editor) {
      return {
        title: Assets.getTranslatedString(editor, 'assets_title'),
        minWidth: 800,
        minHeight: 600,
        contents: [
          {
            id: 'asset_frame',
            label: Assets.getTranslatedString(editor, 'assets_label_search'),
            expand: true,
            elements: [
              {
                type: 'iframe',
                src: Drupal.settings.basePath + Drupal.settings.pathPrefix + 'admin/assets/search?render=popup',
                width: '100%',
                height: '100%',
                id: 'asset_frame_iframe',

                onContentLoad: function () {
                  $(this.getElement().$.contentDocument.body).click(
                    function (event) {
                      var target = event.target, dialog, element, wysiwyg, html;

                      if ($(target).hasClass('assets-item-button')) {
                        var id_arr = target.id.substr(11).split('-'),
                          aid = (id_arr.shift()),
                          type = (id_arr).join('-'),
                          tag_id = [aid, type, new Date().getTime()].join(':');

                        html = Assets.getDataById(tag_id);
                        dialog = CKEDITOR.dialog.getCurrent();
                        if (html) {
                          var editor = dialog.getParentEditor();
                          Assets.insertAssetInEditor(editor, html);

                          if (CKEDITOR.env.gecko && html.search(/<object /i) > 0) {
                            wysiwyg = editor.getMode();
                            wysiwyg.loadData(wysiwyg.getData());
                          }
                        }

                        dialog.hide();
                      }
                    }
                  );

                  // Fix height iframe wrapper issue with ckeditor 4.
                  Assets.adjustDialogHeight();
                }
              }
            ]
          }
        ],
        buttons: [CKEDITOR.dialog.cancelButton]
      };
    },

    getContainer: function (tagId, tag, content) {
      if (tagId && tag && content) {
        var $tempContainer = $(tempContainer);
        $tempContainer.html(content);

        var $asset_div = $tempContainer.children();
        var params = this.getTagData(tag);
        var align = (params.mode == 'full') ? 'none' : params.align;

        if ($asset_div.size()) {
          $asset_div.attr('data-asset-cid', tagId);

          if ((align == 'center') || (align == 'left') || (align == 'right')) {
            // Add special classes for visual feedback in wysiwyg.
            var $image = $asset_div.find('img');
            var $video = $asset_div.find('object');
            if ($image.size() || $video.size()) {
              // Center aligment handler.
              if (align == 'center') {
                if ($image.size()) {
                  $image.css({'margin-left': 'auto', 'margin-right': 'auto', 'display': 'block'});
                }
                else if ($video.size()) {
                  $video.css({'margin-left': 'auto', 'margin-right': 'auto', 'display': 'block'});
                }
              }
              // Left, right handler.
              else {
                if ($image.size()) {
                  $image.css('float', align).parent().siblings('div.field').css('clear', 'both');
                }
                else if ($video.size()) {
                  $video.parents('div.field').css('float', align).next('div.field').css('clear', 'both')
                }
              }
            }
            // Add style to wrapper.
            else {
              if (align == 'center') {
                $asset_div.removeClass('rtecenter').addClass('rtecenter');
              }

              if (align == 'left') {
                $asset_div.removeClass('rteright').addClass('rteleft');
              }

              if (align == 'right') {
                $asset_div.removeClass('rteleft').addClass('rteright');
              }
            }
          }
          // Need for small mode, and none align.
          else {
            $asset_div.removeClass('rteleft rteright rtecenter');
          }
        }
        return tempContainer;
      }
    },

    cache: function (tagId, tag, content) {
      var html = '';
      var container = this.getContainer(tagId, tag, content);
      if (container) {
        html = container.innerHTML;
        tagCache[tagId] = {tag: tag, html: html};
        container.innerHTML = '';
      }
      return html;
    },

    getContentByTag: function (tag) {
      var content = '', tagmatches = [], time, tagId;
      $.ajax({
        type: "POST",
        url: Drupal.settings.basePath + Drupal.settings.pathPrefix + 'admin/assets/get',
        data: {tag: tag},
        async: false,
        success: function (asset_content) {
          if (typeof(asset_content) == null) {
            content = '';
          }
          else {
            content = asset_content;
          }
        }
      });

      tagmatches = tag.match(/\[\[asset:([_a-z0-9]+):([0-9]+)\s\{((.)*?)\}\]\]/);
      time = new Date().getTime();
      tagId = tagmatches[2] + ':' + tagmatches[1] + ':' + time;
      return this.cache(tagId, tag, content);
    },

    getDataById: function (tagId, viewMode, align) {
      if (typeof(tagId) != 'undefined') {
        if (typeof(viewMode) == 'undefined') {
          viewMode = 'default';
        }

        if (typeof(align) == 'undefined') {
          align = 'none';
        }

        var tag = '', content = '';
        $.ajax({
          type: "POST",
          dataType: "json",
          url: Drupal.settings.basePath + Drupal.settings.pathPrefix + 'admin/assets/tag/' + tagId + '/' + viewMode + '/' + align,
          async: false,
          success: function (data) {
            tag = data.tag.replace(/\\"/g, '"');
            content = data.content;
          }
        });

        return this.cache(tagId, tag, content);
      }
    },

    attach: function (content) {
      var matches = content.match(/\[\[asset:([_a-z0-9]+):([0-9]+)\s\{((.)*?)\}\]\]/g),
        tag, im, clean_tag, html = '', cid;

      if (matches) {
        for (im = 0; im < matches.length; im = im + 1) {
          html = '';
          tag = matches[im];
          // @todo: Check that it works, needed because wysiwyg encodes 2 times.
          clean_tag = tag.replace(/&amp;quot;/g, '"');

          // Get from cache.
          for (cid in tagCache) {
            if (tagCache.hasOwnProperty(cid)) {
              if (clean_tag === tagCache[cid].tag) {
                html = this.cache(this.generateId(cid), clean_tag, tagCache[cid].html);
                break;
              }
            }
          }

          // Otherwise get content using ajax and cache it.
          if (!html) {
            html = this.getContentByTag(clean_tag);
          }

          content = content.replace(tag, html);
        }
      }
      return content;
    },

    getEnterElement: function (editor) {
      switch (editor.config.enterMode) {
        case CKEDITOR.ENTER_P:
          return editor.document.createElement('p');
        case CKEDITOR.ENTER_DIV:
          return editor.document.createElement('div');
      }

      return editor.document.createElement('br');
    }
  };

  // CKEditor plugin body.
  CKEDITOR.plugins.add('asset', {
      lang: ['en', 'fr', 'ru'],
      requires: ['htmlwriter', 'iframedialog'],

      // Callbacks.
      replaceAsset: function (tag_id, tag) {
        if (Assets.outdated) {
          $.ajax({
            type: "POST",
            url: Drupal.settings.basePath + Drupal.settings.pathPrefix + 'admin/assets/get/' + tag_id,
            data: {
              tag: tag
            },
            async: false,
            success: function (asset_content) {
              if (typeof(asset_content) == null) {
                asset_content = '';
              }

              var el = Assets.outdated, container = Assets.getContainer(tag_id, tag, asset_content),
                html = container.innerHTML;
              Assets.outdated = null;

              if (html) {
                tagCache[tag_id] = {tag: tag, html: html};
                el.getParent() && el.$.parentNode.replaceChild(container.firstChild, el.$);
              }
              container.innerHTML = '';
            }
          });
        }
      },

      init: function (editor) {
        var path = this.path;

        // Re-attach styles on any editor mode switch to wysiwyg.
        editor.on('mode', function (evt) {
          var editor = evt.editor;
          if (editor.mode == 'wysiwyg') {
            editor.document.appendStyleSheet(path + 'assets-editor.css');
          }
        });

        // CKeditor instanceReady event.
        editor.on('instanceReady', function (evt) {
          var editor = evt.editor;
          if (CKEDITOR.instances && CKEDITOR.env) {
            // For webkit set cursor of wysiwyg to the end to prevent asset in asset pasting.
            if (CKEDITOR.env.webkit) {
              // Handle case for CKEditor 4.
              if (Assets.getCKeditorVersion() >= 4) {
                // Create a range for the entire contents of the editor document body.
                var range = editor.createRange();
                // Move to the end of the range.
                range.moveToPosition(range.root, CKEDITOR.POSITION_BEFORE_END);
                // Putting the current selection there.
                editor.getSelection().selectRanges([range]);
              }
            }

            // Fix for CKEditor 3 & Chrome and for CKEditor 4 & FF.
            if ((Assets.getCKeditorVersion() < 4 && CKEDITOR.env.webkit)
              || (Assets.getCKeditorVersion() >= 4 && CKEDITOR.env.gecko)) {

              // Getting selection.
              var selected = editor.getSelection();
              // Getting ranges.
              var selected_ranges = selected.getRanges();
              // Selecting the starting node.
              var range = selected_ranges[0];

              if (range) {
                var node = range.startContainer;
                var parents = node.getParents(true);

                node = parents[parents.length - 2].getFirst();
                if (node) {
                  while (true) {
                    var x = node ? node.getNext() : null;

                    if (x == null) {
                      break;
                    }

                    node = x;
                  }

                  selected.selectElement(node);
                }

                selected_ranges = selected.getRanges();
                // False collapses the range to the end of the selected node, true before the node.
                selected_ranges[0].collapse(false);
                // Putting the current selection there.
                selected.selectRanges(selected_ranges);
              }
            }
          }
        });

        editor.on('key', function (evt) {
          var editor = evt.editor;
          if (editor.readOnly)
            return true;

          var keyCode = evt.data.keyCode, isHandled;
          // Backspace OR Delete.
          if (keyCode in { 8: 1, 46: 1 }) {
            var sel = editor.getSelection(),
              selected,
              range = sel.getRanges()[ 0 ],
              rtl = (keyCode == 8), element, suggestedElement;

            if (!range) {
              return true;
            }

            // Remove the entire asset on fully selected content.
            if (Assets.selectedElement) {
              element = Assets.selectedElement;
            }
            else if (range.collapsed && range.startContainer) {
              var isNotWhitespace = CKEDITOR.dom.walker.whitespaces(true);
              // Handle the following special cases:
              // If we are on text node, but it contains only zero-width space (&#8203;).
              if (range.startContainer.type === CKEDITOR.NODE_TEXT && (text = range.startContainer.getText())
                && text.length === 1 && text.charCodeAt() === 8203
                && (suggestedElement = range.startContainer[ rtl ? 'getPrevious' : 'getNext' ](isNotWhitespace))
                && suggestedElement.$.attributes && suggestedElement.$.attributes['data-asset-cid']) {
                element = suggestedElement;
              }
              // We pressing key within body - we are on empty line (no text node or etc).
              else if (range.startContainer.type === CKEDITOR.NODE_ELEMENT && range.startContainer.getName() == 'body'
                && (suggestedElement = range.startContainer.getChild(rtl ? range.startOffset - 1 : range.startOffset + 1))
                && suggestedElement.$.attributes && suggestedElement.$.attributes['data-asset-cid']) {
                element = suggestedElement;
              }
            }

            if (element) {
              editor.fire('saveSnapshot');
              range.moveToPosition(element, rtl ? CKEDITOR.POSITION_BEFORE_START : CKEDITOR.POSITION_AFTER_END);
              element.remove();
              range.select();
              editor.fire('saveSnapshot');

              Assets.deselect();

              isHandled = 1;
            }
          }

          return !isHandled;
        });

        // Wrapper for contentDom group events.
        editor.on('contentDom', function (evt) {
          editor.document.on('click', function (evt) {
            var element = evt.data.getTarget();
            var original_element = element;

            while (element && !(element.type === CKEDITOR.NODE_ELEMENT && element.data('asset-cid'))) {
              element = element.getParent();
            }

            if (element) {
              var selection = editor.getSelection();
              var range = new CKEDITOR.dom.range(selection.root);
              range.setStartBefore(element);
              range.setEndAfter(element);
              range.shrink(CKEDITOR.SHRINK_TEXT);
              selection.selectRanges([ range ]);

              Assets.select(element);
              if (original_element.is('img')) {
                evt.data.preventDefault(true);
              }
            }
            else {
              Assets.deselect();
            }
          });

          editor.document.on('mousedown', function (evt) {
            var element = evt.data.getTarget();

            if (element.is('img')) {
              while (element && !(element.type === CKEDITOR.NODE_ELEMENT && element.data('asset-cid'))) {
                element = element.getParent();
              }
              if (element) {
                evt.data.preventDefault(true);
              }
            }
          });
        });

        // Paste event.
        editor.on('paste', function (evt) {
          var data = evt.data, dataProcessor = new pasteProcessor(), htmlFilter = dataProcessor.htmlFilter,
            processed = {};

          htmlFilter.addRules({
            elements: {
              'div': function (element) {
                var wrapper, tagId, tag_id;
                Assets.deselect(element);

                if (element.attributes && element.attributes['data-asset-cid']) {
                  // @todo: Check for webkit this functionality is forbidden.
                  if (CKEDITOR.env.webkit) {
                    return false;
                  }

                  tag_id = element.attributes['data-asset-cid'];

                  if (!processed[tag_id]) {
                    tagId = Assets.generateId(tag_id);

                    if (typeof(tagCache[tag_id]) === 'undefined') {
                      Assets.getDataById(tagId);
                    }

                    processed[tagId] = 1;
                    wrapper = new CKEDITOR.htmlParser.fragment.fromHtml(tagCache[tagId].html);

                    return wrapper.children[0];
                  }
                }
                return element;
              }
            }
          });

          if (typeof data['html'] !== 'undefined') {
            try {
              data['html'] = dataProcessor.toHtml(data['html']);
            }
            catch (e) {
              if (typeof(console) !== 'undefined') {
                console.log(Assets.getTranslatedString(editor, 'assets_error_paste'));
              }
            }
          }
          Assets.deselect();
        }, this);

        // Double click event.
        editor.on('doubleclick', function (evt) {
          var editor = evt.editor;

          // Getting selection.
          var element = Assets.getSelected(editor), tag_id, tag, src;

          // Open dialog frame.
          if (element) {
            Assets.outdated = element;
            tag_id = element.data('asset-cid');
            tag = encodeURIComponent(tagCache[tag_id].tag);
            src = Drupal.settings.basePath + Drupal.settings.pathPrefix + 'admin/assets/override?render=popup&tag=' + tag;
            Assets.openDialog(editor, 'asset_' + Assets.parseId(tag_id, 'type'), src, element);
          }
        });

        // Common functionality for the plugin.
        this.Assets = Assets;

        var conf = Drupal.settings.ckeditor.plugins.asset, assetType, type, execFn;
        if (!conf) {
          return;
        }

        for (assetType in conf) {
          if (conf.hasOwnProperty(assetType)) {
            // todo: Type used for internal dialog and command name instead of original name, we need to simplify it.
            type = 'asset_' + assetType;
            CKEDITOR.dialog.add(type, Assets.dialog(editor, type));

            execFn = function (type, assetType) {
              return function (editor) {
                Assets.openDialog(editor, type, Drupal.settings.basePath + Drupal.settings.pathPrefix + 'admin/assets/add/' + assetType + '/?render=popup', null);
              };
            };

            editor.addCommand(type, {
              exec: execFn(type, assetType),
              canUndo: false,
              editorFocus: CKEDITOR.env.ie || CKEDITOR.env.webkit
            });

            editor.ui.addButton && editor.ui.addButton(type, {
              label: conf[assetType].name,
              command: type,
              icon: this.path + 'buttons/' + conf[assetType].icon
            });
          }
        }

        // Add commands for asset.
        editor.addCommand('addLineAfter', {
          exec: function (editor) {
            var node = Assets.getSelected(editor), newline;
            if (node) {
              Assets.deselect();
              newline = Assets.getEnterElement(editor);
              newline.insertAfter(node);
              editor.getSelection().selectElement(newline);
            }
          },
          canUndo: true
        });

        editor.addCommand('addLineBefore', {
          exec: function (editor) {
            var node = Assets.getSelected(editor), newline;

            if (node) {
              Assets.deselect();
              newline = Assets.getEnterElement(editor);
              newline.insertBefore(node);
              editor.getSelection().selectElement(newline);
            }
          },
          canUndo: true
        });

        CKEDITOR.dialog.add('assetSearch', Assets.searchDialog);
        editor.addCommand('assetSearch', new CKEDITOR.dialogCommand('assetSearch'));
        editor.ui.addButton && editor.ui.addButton('assetSearch', {
          label: Assets.getTranslatedString(editor, 'assets_btn_search'),
          command: 'assetSearch',
          icon: this.path + 'search.png'
        });

        editor.addCommand('assetOverride', {
          exec: function (editor) {
            var element = Assets.getSelected(editor), tag_id, tag, src;
            if (element) {
              Assets.outdated = element;
              tag_id = element.data('asset-cid');
              tag = encodeURIComponent(tagCache[tag_id].tag);
              src = Drupal.settings.basePath + Drupal.settings.pathPrefix + 'admin/assets/override?render=popup&tag=' + tag;
              Assets.openDialog(editor, 'asset_' + Assets.parseId(tag_id, 'type'), src, element);
            }
          },
          canUndo: false,
          editorFocus: CKEDITOR.env.ie || CKEDITOR.env.webkit
        });

        editor.addCommand('assetEdit', {
          exec: function (editor) {
            var element = Assets.getSelected(editor);
            if (element) {
              Assets.outdated = element;

              var tag_id = element.data('asset-cid');
              var params = Assets.getTagData(tagCache[tag_id].tag);
              var src = [
                Drupal.settings.basePath + Drupal.settings.pathPrefix + 'admin/assets/edit',
                params.aid,
                params.mode,
                params.align,
                '?render=popup'
              ].join('/');

              Assets.openDialog(editor, 'asset_' + Assets.parseId(tag_id, 'type'), src, element);
            }
          },
          canUndo: false,
          editorFocus: CKEDITOR.env.ie || CKEDITOR.env.webkit
        });

        editor.addCommand('assetDelete', {
          exec: function (editor) {
            Assets.removeAssetFromEditor(editor, Assets.getSelected(editor));
          },
          canUndo: true,
          editorFocus: CKEDITOR.env.ie || CKEDITOR.env.webkit
        });

        editor.addCommand('assetCut', {
          exec: function (editor) {
            Assets.removeAssetFromEditor(editor, Assets.getSelected(editor), true);
          },
          canUndo: true,
          editorFocus: CKEDITOR.env.ie || CKEDITOR.env.webkit
        });

        editor.addCommand('assetPaste', {
          exec: function (editor) {
            if (cutted !== null) {
              Assets.deselect(cutted);
              editor.insertElement(cutted);
              cutted = null;
            }
          },
          canUndo: true,
          editorFocus: CKEDITOR.env.ie || CKEDITOR.env.webkit
        });

        // Create context menu.
        if (editor.addMenuItem) {
          editor.addMenuGroup('asset_operations', 100);

          editor.addMenuItems({
            assetcut: {
              label: Assets.getTranslatedString(editor, 'assets_cut'),
              command: 'assetCut',
              group: 'asset_operations',
              icon: this.path + 'cut.png',
              order: 1
            },
            assetpaste: {
              label: Assets.getTranslatedString(editor, 'assets_paste'),
              command: 'assetPaste',
              group: 'asset_operations',
              icon: this.path + 'paste.png',
              order: 2
            },
            assetdelete: {
              label: Assets.getTranslatedString(editor, 'assets_delete'),
              command: 'assetDelete',
              group: 'asset_operations',
              icon: this.path + 'delete.png',
              order: 3
            }
          });

          editor.addMenuGroup('asset_newline', 200);
          editor.addMenuItems({
            addLineBefore: {
              label: Assets.getTranslatedString(editor, 'assets_nl_before'),
              command: 'addLineBefore',
              group: 'asset_newline',
              order: 1
            },
            addLineAfter: {
              label: Assets.getTranslatedString(editor, 'assets_nl_after'),
              command: 'addLineAfter',
              group: 'asset_newline',
              order: 2
            }
          });

          editor.addMenuGroup('asset_edit', 300);
          editor.addMenuItems({
            assetoverride: {
              label: Assets.getTranslatedString(editor, 'assets_override'),
              command: 'assetOverride',
              group: 'asset_edit',
              icon: this.path + 'override.png',
              order: 1
            },
            assetedit: {
              label: Assets.getTranslatedString(editor, 'assets_edit'),
              command: 'assetEdit',
              group: 'asset_edit',
              icon: this.path + 'edit.png',
              order: 2
            }
          });
        }

        if (editor.contextMenu) {
          editor.contextMenu.addListener(function (element, selection) {
            var type, conf, menu = {};
            var editor = selection.root.editor;

            while (element && !(element.type === CKEDITOR.NODE_ELEMENT && element.data('asset-cid'))) {
              element = element.getParent();
            }

            // Open context menu.
            if (element) {
              // If we are on asset with contextual menu - deny any other options like paste and etc.
              editor.contextMenu.removeAll();

              // Select asset element, if element wasn't selected before.
              if (!element.hasClass('selected')) {
                Assets.select(element)
              }

              type = Assets.parseId(element.data('asset-cid'), 'type');
              conf = Drupal.settings.ckeditor.plugins.asset[type];

              var suppressedMenuItems = element.data('asset-suppress-menu-items');
              suppressedMenuItems = suppressedMenuItems ? suppressedMenuItems.split(' ') : [];

              // Add default menu items.
              var defaultMenuItems = ['assetcut', 'assetdelete', 'addLineBefore', 'addLineAfter'];
              $.each(defaultMenuItems, function(key, menuItem) {
                if ($.inArray(menuItem, suppressedMenuItems) === -1) {
                  menu[menuItem] = CKEDITOR.TRISTATE_ON;
                }
              });

              // We just control item visibility, real access check will be on server side.
              if ($.inArray('assetedit', suppressedMenuItems) === -1 && conf.accessEdit) {
                menu.assetedit = CKEDITOR.TRISTATE_ON;
              }

              // Check visibility for "override" item.
              if ($.inArray('assetoverride', suppressedMenuItems) === -1 && conf && conf.modes) {
                if (!(Object.keys(conf.modes).length === 1 && conf.modes.full && !conf.fields.length)) {
                  menu.assetoverride = CKEDITOR.TRISTATE_ON;
                }
              }
            }
            else {
              if (cutted !== null) {
                menu = {assetpaste: CKEDITOR.TRISTATE_ON};
              }
            }

            return menu;
          });
        }

        // The paste processor here is just a reduced copy of html data processor.
        var pasteProcessor = function () {
          this.htmlFilter = new CKEDITOR.htmlParser.filter();
        };

        pasteProcessor.prototype = {
          toHtml: function (data) {
            var fragment = CKEDITOR.htmlParser.fragment.fromHtml(data, false),
              writer = new CKEDITOR.htmlParser.basicWriter();

            fragment.writeHtml(writer, this.htmlFilter);
            return writer.getHtml(true);
          }
        };
      },

      afterInit: function (editor) {
        // Register a filter to displaying placeholders after mode change.
        var dataProcessor = editor.dataProcessor,
          dataFilter = dataProcessor && dataProcessor.dataFilter,
          htmlFilter = dataProcessor && dataProcessor.htmlFilter,
          HtmlDPtoHtml = dataProcessor && editor.dataProcessor.toHtml;

        if (HtmlDPtoHtml) {
          // Unprotect some flash tags, force democracy.
          editor.dataProcessor.toHtml = function (data, fixForBody) {
            var unprotectFlashElementNamesRegex = /(<\/?)cke:((?:object|embed|param)[^>]*>)/gi;
            data = HtmlDPtoHtml.apply(editor.dataProcessor, [data, fixForBody]);

            return data.replace(unprotectFlashElementNamesRegex, '$1$2');
          };
        }

        if (dataFilter) {
          dataFilter.addRules({
            text: function (text) {
              return Assets.attach(text);
            }
          });
        }

        if (htmlFilter) {
          htmlFilter.addRules({
            elements: {
              'div': function (element) {
                if (element.attributes && element.attributes['data-asset-cid']) {
                  var tag_id = element.attributes['data-asset-cid'];

                  var tag = tagCache[tag_id].tag;
                  tag = tag.replace(/</g, '&lt;');
                  tag = tag.replace(/>/g, '&gt;');

                  var tagEl = new CKEDITOR.htmlParser.fragment.fromHtml(tag);
                  return tagEl.children[0];
                }

                return element;
              }
            }
          },
          {
            applyToAll: true
          });
        }
      }
    }
  );
})(jQuery);
