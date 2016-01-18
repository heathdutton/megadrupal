CKEDITOR.dialog.add('embridgeDialog', function(editor) {
    var prefwidth = window.innerWidth * 70 / 100;
    var prefheight = window.innerHeight * 70 / 100;
    var search_url = '/embridge/search/' + Drupal.settings.embridge.default_catalog + '?clear_frame=true&embed=true&display_type=thumbnail';
    var upload_url = '/embridge/upload/' + Drupal.settings.embridge.default_catalog + '?clear_frame=true&embed=true';
    var conversions = new Array();
    conversions[0] = ['thumb'];
    var i = 0;
    for(var rendition in Drupal.settings.embridge.selected_wysiwyg_renditions) {
        conversions[i] = [rendition];
        i++;
    }
    var contents = new Array();
    var i = 0;
    if (Drupal.settings.embridge.allow_search) {
        contents[i] = {
                id: 'search-asset',
                label: 'Search Asset',
                elements: [
                  {
                    type : 'text',
                    id : 'title',
                    label : 'Title'
                  },
                  {
                    type: 'select',
                    id: 'conversion',
                    label: 'Conversion',
                    items: conversions,
                    'default': 'thumb'
                  },
                  {
                     type : 'iframe',
                     src : search_url,
                     width : prefwidth,
                     height : prefheight,
                     onContentLoad : function() {
                        // Iframe is loaded...
                        var iframe = document.getElementById(this._.frameId);
                        iframeWindow_Search = iframe.contentWindow;
                     }
                  }
                ]
        };
        i++;
    }
    if (Drupal.settings.embridge.allow_upload) {
        contents[i]  = {
            id: 'upload-asset',
            label: 'Upload Asset',
            elements: [
              {
                type : 'text',
                id : 'title',
                label : 'Title'
              },
              {
                type: 'select',
                id: 'conversion',
                label: 'Conversion',
                items: conversions,
                'default': 'thumb'
              },
              {
                 type : 'iframe',
                 src : upload_url,
                 width : prefwidth,
                 height : prefheight,
                 onContentLoad : function() {
                    // Iframe is loaded...
                    var iframe = document.getElementById(this._.frameId);
                    iframeWindow_Upload = iframe.contentWindow;
                 }
              }
            ]
        };
        i++;
    }

    if (contents.length == 0) {
        contents[i]  = {
                    id: 'warning',
                    label: 'Warning',
                    elements: [
                      {
                        type : 'html',
                        html : '<span style="color:red">You do not have permission to add assets from EnterMedia.</span>'
                      },
                    ]
                };
    }
    return {
        title: 'Add EnterMedia Image',
        minWidth: prefwidth,
        minHeight: prefheight,
        contents: contents,
        onOk: function() {
            var dialog = this;
            var asset;
            if (this._.currentTabId == 'search-asset') {
                if (typeof(iframeWindow_Search.embridge_get_selected_assets) == 'function') {
                    var assets = iframeWindow_Search.embridge_get_selected_assets();
                    if (assets.length > 0) {
                        asset = assets[0];
                    }
                }
            }
            else if (this._.currentTabId == 'upload-asset') {
                if (typeof(iframeWindow_Upload.embridge_get_uploaded_asset) == 'function') {
                    asset = iframeWindow_Upload.embridge_get_uploaded_asset();
                }
            }
            if (asset) {
                var image = editor.document.createElement('img');
                var title = dialog.getValueOf(this._.currentTabId, 'title');
                if (title) {
                    image.setAttribute('title', title);
                }
                var conversion = dialog.getValueOf(this._.currentTabId, 'conversion');
                if (asset[conversion]) {
                    image.setAttribute('src', asset[conversion]);
                }
                else {
                    image.setAttribute('src', asset['thumbnail']);
                }
                editor.insertElement(image);
            }
        }
    };
});
var iframeWindow_Search = null;
var iframeWindow_Upload = null;