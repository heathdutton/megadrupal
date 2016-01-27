/*
 * Ext JS Alfresco Browser for Drupal.
 * 
 * Module Pattern
 * http://extjs.com/learn/Manual:Basic_Application_Design
 */
Ext.ns('AlfrescoBrowser');

AlfrescoBrowser.ShowMsgError = function (title, msg) {
  Ext.Msg.show({
    title: title,
    msg: msg,
    minWidth: 200,
    modal: true,
    icon: Ext.MessageBox.ERROR,
    buttons: Ext.Msg.OK
  });
}

AlfrescoBrowser.ShowMsgWarning = function (title, msg) {
  Ext.Msg.show({
    title: title,
    msg: msg,
    minWidth: 200,
    modal: true,
    icon: Ext.MessageBox.WARNING,
    buttons: Ext.Msg.OK
  });
}

AlfrescoBrowser.SendItem = function (itemsGrid) {
  var items = itemsGrid.getSelectionModel().getSelections();
  if (items.length == 0 || !window.opener) {
    return;
  }
  
  var node = items[0].data;
  var reference = 'workspace://SpacesStore/' + node.id;
  var title = Ext.util.Format.trim(node.title);
  if (title.length == 0) {
    title = Ext.util.Format.trim(node.name);
  }

  var params = Ext.urlDecode(location.search.substring(1));
  if (params['r']) {
    window.opener.document.getElementById(params['r']).value = reference;
  }
  if (params['n']) {
    window.opener.document.getElementById(params['n']).value = node.name;
  }

  // Node edit form 
  var nodeTitle = window.opener.document.getElementById('edit-title');
  if (nodeTitle.value.length == 0 && title.length > 0) {
    nodeTitle.value = title;
  }
  
  self.close();
}

AlfrescoBrowser.ViewItem = function (itemsGrid) {
  var items = itemsGrid.getSelectionModel().getSelections();
  if (items.length == 0) {
    return;
  }

  var node = items[0].data;
  var url = Drupal.settings.alfresco.serviceDownloadUrl + "/" + node.name + "?node=" + node.id;
  var size = Ext.getCmp('alfresco-browser-viewport').getSize();
  
  var iframeWin = new Ext.Window({
    id: 'preview-window',
    title: node.name,
    width: size.width - 100,
    height: size.height - 100,
    maximizable: true,
    modal: 'true',
    layout: 'fit',
    html: '<iframe id="preview-frame" frameborder="0"  src="' + url + '" onLoad="AlfrescoBrowser.ViewItemOnLoad()"></iframe>',
    buttonAlign: 'center',
    defaultButton: 0,
    animateTarget: 'btn-open',
    buttons: [{
      text: 'Close',
      handler: function() {
        iframeWin.close();
      }
    }],
    listeners: {
      'afterrender': function(win) {
        var mask = new Ext.LoadMask(win.body);
        mask.show();
      }
    }
  });
  
  iframeWin.show();
}

AlfrescoBrowser.ViewItemOnLoad = function () {
  var iframeWin = Ext.getCmp('preview-window');
  if (iframeWin) {
    iframeWin.body.unmask();
  }
}

AlfrescoBrowser.DownloadItem = function (itemsGrid, forceDownload) {
  var items = itemsGrid.getSelectionModel().getSelections();
  if (items.length == 0) {
    return;
  }

  var node = items[0].data;
  var url = Drupal.settings.alfresco.serviceDownloadUrl + "/" + node.name + "?node=" + node.id;
  
  if (forceDownload) {
    url += "&mode=attachment";
  }
  
  if (Ext.isIE) {
    if (forceDownload) {
      window.location = url;
    } else if (top.location.href != window.location.href) {
      top.open(url);
    } else {
      window.open(url);
    }
  } else {
    window.open(url);
  }
}

AlfrescoBrowser.DeleteItem = function (itemsGrid, folderTree) {
  var id = null;
  var name = null;
  var space = null;
  var items = itemsGrid.getSelectionModel().getSelections();
  if (items.length != 0) {
    id = items[0].data.id;
    name = items[0].data.name;
  }
  else {
    var space = folderTree.getSelectionModel().getSelectedNode();
    if (!Ext.isEmpty(space)) {
      id = space.id;
      name = space.text;
    }
  }
  if (!id) {
    return;
  }
  itemsGrid.disable();
  
  var confirmMsg = '';
  if (space) {
    confirmMsg = Drupal.t('Are you sure you want to delete "!name" space and all its contents?', {'!name' : name});
  }
  else {
    confirmMsg = Drupal.t('Are you sure you want to delete "!name" and all previous versions?', {'!name' : name});
  }
  Ext.MessageBox.confirm(
    space ? Drupal.t('Confirm Delete Space') : Drupal.t('Confirm Delete Item'),
    confirmMsg,
    function(btn) {
      if (btn == 'yes') {
        var url = Drupal.settings.alfresco.serviceDeleteUrl + "/" + name + "?node=" + id + "&token=" + Drupal.settings.alfresco.token;
        Ext.Ajax.request({
          url: url,
          success: function(response, options) {
            var result = Ext.decode(response.responseText);
            if (result.success) {
              if (space) {
                parentNode = space.parentNode;
                parentNode.reload();
                folderTree.selectPath(parentNode.getPath());
                itemsGrid.store.baseParams = {node: parentNode.id};
                itemsGrid.store.load({params:{start:0}});
                itemsGrid.setTitle(name);
              } else {
                itemsGrid.store.load({params:{start: 0, cache: 'node'}});
              }
            } else {
              AlfrescoBrowser.ShowMsgWarning(Drupal.t('Warning'), result.error);
            }
            itemsGrid.enable();
          },
          failure: function() {
            AlfrescoBrowser.ShowMsgError(Drupal.t('Fail'), Drupal.t('Request failed.'));
            itemsGrid.enable();
          }
        });
      } else {
        itemsGrid.enable();
      }
  });
}

AlfrescoBrowser.AddItem = function (folderTree, dataStore) {
  var space = folderTree.getSelectionModel().getSelectedNode();
  if (Ext.isEmpty(space)) {
    return;
  }

  var uploadForm = new Ext.FormPanel({
    fileUpload: true,
    width: 500,
    frame: true,
    border: false,
    autoHeight: true,
    bodyStyle: 'padding: 10px 10px 0 10px;',
    labelWidth: 50,
    monitorValid: true,
    defaults: {
      anchor: '95%',
      allowBlank: false,
      msgTarget: 'side'
    },
    items: [{
      xtype: 'fileuploadfield',
      id: 'form-file',
      name: 'files[file]',  // No change: required for Drupal Form API
      emptyText: Drupal.t('Locate content to upload (max. upload size: !size)', {'!size' : Drupal.settings.alfresco.maxUploadSize}),
      fieldLabel: Drupal.t('File'),
      labelStyle: 'font-weight:bold;',
      listeners: {
        'fileselected': function(fb, value) {
          if (value) {
            var filename = value.replace(/^.*\\/, '')
            Ext.getCmp('form-name').setValue(filename);
          }
        }
      }
    }, {
      xtype: 'textfield',
      id: 'form-name',
      name: 'name',
      fieldLabel: Drupal.t('Name'),
      labelStyle: 'font-weight:bold;',
      maxLength: 255
    }, {
      xtype: 'hidden',
      name: 'space',
      value: space.id
    }, {
      xtype: 'fieldset',
      title: Drupal.t('Content properties'),
      defaultType: 'textfield',
      labelWidth: 70,
      anchor: '100%',
      style: 'margin-top: 10px;',
      defaults: {
        anchor: '95%',
        msgTarget: 'side'
      },
      autoHeight:true,
      items: [{
        fieldLabel: Drupal.t('Title'),
        name: 'title',
        allowBlank: false,
        labelStyle: 'font-weight:bold;',
        maxLength: 255,
        listeners: {
          afterrender: function(field) {
            field.focus(false, 500);
          }
        }
      },{
        xtype: 'textarea',
        fieldLabel: Drupal.t('Description'),
        name: 'description'
      },{
        fieldLabel: Drupal.t('Author'),
        name: 'author',
        maxLength: 255
      }]
    }],
    buttons: [{
      text: Drupal.t('Add'),
      formBind: true,
      handler: function() {
        if (uploadForm.getForm().isValid()) {
          uploadForm.getForm().submit({
            url: Drupal.settings.alfresco.serviceUploadUrl,
            waitMsg: Drupal.t('Uploading your content...'),
            success: function(form, o) {
              dataStore.load({params:{start: 0, cache: 'node'}});
              uploadWindow.close();
            },
            failure: function(form, o){
              Ext.Msg.show({
                title: Drupal.t('An error has occurred.'),
                msg: o.result.error,
                minWidth: 200,
                modal: true,
                icon: Ext.MessageBox.WARNING,
                buttons: Ext.Msg.OK
              });
            }
          });
        }
      }
    }, {
      text: 'Cancel',
      handler: function(){
        uploadWindow.close();
      }
    }]
  });

  var uploadWindow = new Ext.Window({
    id: 'upload-window',
    title: Drupal.t('Add content to current space: !space', {'!space' : space.text}),
    autoHeight: true,
    width: 500,
    minWidth: 300,
    modal: 'true',
    layout: 'fit', 
    animateTarget: 'btn-add',
    items: [ uploadForm ]
  });
  
  uploadWindow.show();
}

AlfrescoBrowser.CreateSpace = function (folderTree) {
  var space = folderTree.getSelectionModel().getSelectedNode();
  if (Ext.isEmpty(space)) {
    return;
  }

  var createSpaceForm = new Ext.FormPanel({
    width: 500,
    frame: true,
    border: false,
    autoHeight: true,
    bodyStyle: 'padding: 10px 10px 0 10px;',
    labelWidth: 70,
    monitorValid: true,
    defaults: {
      anchor: '95%',
      allowBlank: false,
      msgTarget: 'side'
    },
    items: [{
      xtype: 'textfield',
      id: 'form-name',
      name: 'name',
      allowBlank: false,
      fieldLabel: Drupal.t('Name'),
      labelStyle: 'font-weight:bold;',
      maxLength: 255,
      listeners: {
        afterrender: function(field) {
          field.focus(false, 500);
        }
      }
    }, {
      xtype: 'hidden',
      name: 'space',
      value: space.id
    }, {
      xtype: 'textfield',
      fieldLabel: Drupal.t('Title'),
      name: 'title',
      allowBlank: true,
      maxLength: 255
    },{
      xtype: 'textarea',
      fieldLabel: Drupal.t('Description'),
      name: 'description',
      allowBlank: true
    }],
    buttons: [{
      text: Drupal.t('Create Space'),
      formBind: true,
      handler: function() {
        if (createSpaceForm.getForm().isValid()) {
          createSpaceForm.getForm().submit({
            url: Drupal.settings.alfresco.serviceCreateUrl,
            waitMsg: Drupal.t('Creating space...'),
            success: function(form, o) {
              space.reload();
              createSpaceWindow.close();
            },
            failure: function(form, o){
              Ext.Msg.show({
                title: Drupal.t('An error has occurred.'),
                msg: o.result.error,
                minWidth: 200,
                modal: true,
                icon: Ext.MessageBox.WARNING,
                buttons: Ext.Msg.OK
              });
            }
          });
        }
      }
    }, {
      text: 'Cancel',
      handler: function(){
        createSpaceWindow.close();
      }
    }]
  });

  var createSpaceWindow = new Ext.Window({
    id: 'create-space-window',
    title: Drupal.t('Create space on: !space', {'!space' : space.text}),
    autoHeight: true,
    width: 500,
    minWidth: 300,
    modal: 'true',
    layout: 'fit', 
    animateTarget: 'btn-add-folder',
    items: [ createSpaceForm ]
  });
  
  createSpaceWindow.show();
}

AlfrescoBrowser.App = function() {
  var folderTree;
  var itemsGrid;
  var propsGrid;
  var dataStore;
  var currentPath;
  var firstRun = true;

  return {
    init: function() {
      Ext.Ajax.timeout = 60000; // 60s (default 30s)
      Ext.state.Manager.setProvider(new Ext.state.CookieProvider());
      Ext.QuickTips.init();
      
      this.initFolderTree();
      this.initDocumentGrid();
      this.initSearch();
      this.initLayout();
      
      setTimeout(function() {
        Ext.get('loading').remove();
        Ext.fly('loading-mask').fadeOut({
            remove: true
        });
      }, 250);
      
      // @todo Mostrar ventana informando en caso de error.
    },
    initLayout: function(){
      // --------------------------------------------
      // -- LAYOUT
      // --------------------------------------------
      var viewport = new Ext.Viewport({
        id: 'alfresco-browser-viewport',
        layout: 'border',
        items: [new Ext.BoxComponent({
          region: 'north',
          el: 'header',
          height: 35
        }), folderTree, {
          region: 'center',
          layout: 'border',
          border: false,
          margins: '5 5 5 0',
          items: [ itemsGrid, propsGrid ]
        }]
      });
    },
    initFolderTree: function(){
      // --------------------------------------------
      // -- NAVIGATION TREE
      // --------------------------------------------
      folderTree = new Ext.tree.TreePanel({
        id: 'folder-tree',
        region: 'west',
        collapsible: true,
        collapseMode: 'mini',
        hideCollapseTool: true,
        title: Drupal.t('Browse Spaces'),
        margins: '5 0 5 5',
        cmargins: '5 5 5 5',
        width: 240,
        minSize: 100,
        split: true,
        layout: 'fit',
        autoScroll: true,
        
        // tree specifics
        rootVisible: true,
        useArrows: true,
        trackMouseOver: false,

        loader: new Ext.tree.TreeLoader({
          dataUrl: Drupal.settings.alfresco.serviceTreeUrl,
          requestMethod: 'GET'
        }),
        
        root: new Ext.tree.AsyncTreeNode({
          text: Drupal.settings.alfresco.homeText,
          id: Drupal.settings.alfresco.homeRef,
          expanded: true,
          listeners: {
            'load': function() {
              if (firstRun) {
                if (Drupal.settings.alfresco.folderPath != '') {
                  folderTree.expandPath(Drupal.settings.alfresco.folderPath);
                  folderTree.selectPath(Drupal.settings.alfresco.folderPath);
                }
                firstRun = false;
              }
              else if (currentPath != '') {
                folderTree.expandPath(currentPath);
                folderTree.selectPath(currentPath);
              }
          }}
        }),
        
        listeners: {
          'click': function(node, e) {
    	  	if (dataStore.baseParams.node != node.id) {
	            dataStore.baseParams = {node: node.id};
	            dataStore.load({params:{start:0}});
	            itemsGrid.setTitle(node.text);
	            //node.expand();
    	  	}
          if (Drupal.settings.alfresco.accessAdd) {
            Ext.getCmp('btn-add').enable();
            Ext.getCmp('btn-add-folder').enable();
          }
          if (Drupal.settings.alfresco.accessDelete) {
            Ext.getCmp('btn-delete').enable();
          }
        }},
        
        tools: [{
          id: 'refresh',
          on: {
            click: function(){
              var currentNode = folderTree.getSelectionModel().getSelectedNode();
              if (Ext.isEmpty(currentNode)) {
                currentNode = folderTree.root;
              }
              currentPath = currentNode.getPath();
              
              var baseParams = folderTree.loader.baseParams || {};
              folderTree.loader.baseParams['cache'] = 'all';
              folderTree.root.reload();
              folderTree.loader.baseParams = baseParams;
            }}
        }]
      });
    },
    initSearch: function(){
      // --------------------------------------------
      // -- SEARCH
      // --------------------------------------------
      Ext.app.SearchField = Ext.extend(Ext.form.TwinTriggerField, {
        initComponent : function(){
            Ext.app.SearchField.superclass.initComponent.call(this);
            this.on('specialkey', function(f, e){
                if(e.getKey() == e.ENTER){
                    this.onTrigger2Click();
                }
            }, this);
        },

        validationEvent:false,
        validateOnBlur:false,
        trigger1Class:'x-form-clear-trigger',
        trigger2Class:'x-form-search-trigger',
        hideTrigger1: true,
        hasSearch: false,
        paramName: 'query',
        
        onTrigger1Click : function(){
            if(this.hasSearch){
                this.el.dom.value = '';
                var node = folderTree.getSelectionModel().getSelectedNode();
                if (Ext.isEmpty(node)) {
                  node = folderTree.root;
                }
                this.store.baseParams = {node:node.id};
                this.store.load({params:{start:0}});
                this.triggers[0].hide();
                this.hasSearch = false;
                itemsGrid.setTitle(node.text);
            }
        },

        onTrigger2Click : function(){
            var v = this.getRawValue();
            if (v.length < 1) {
              this.onTrigger1Click();
              return;
            }
            if (v.length < 3) {
              return;
            }
            var title = Drupal.t('Search items: !query', {'!query': Ext.util.Format.htmlEncode(v)});
            itemsGrid.setTitle(title);
            Ext.getCmp('btn-add').disable();
            Ext.getCmp('btn-add-folder').disable();
            Ext.getCmp('btn-delete').disable();

            this.store.baseParams[this.paramName] = v;
            this.store.load({params:{start:0}});
            this.hasSearch = true;
            this.triggers[0].show();
        }
      });

      var search = new Ext.app.SearchField({
        region: 'west',
        store: dataStore,
        width: 320,
        emptyText: Drupal.t('Search (minimum 3 characters)'),
        applyTo: 'search',
        hidden: Drupal.settings.alfresco.searchMode == 0 ? true : false
      })
    },
    initDocumentGrid: function(){
      // --------------------------------------------
      // -- CONTENT GRID
      // --------------------------------------------
      var record = Ext.data.Record.create([
        {name: 'id'},   /* alfresco node id */
        {name: 'nid'},  /* drupal node id */
        {name: 'type'},
        {name: 'name'},
        {name: 'path'},
        {name: 'icon'},
        {name: 'author'},
        {name: 'creator'},
        {name: 'title'},
        {name: 'description'},
        {name: 'size'},
        {name: 'mimetype'},
        {name: 'created', type: 'date', dateFormat: 'Y-m-d H:i:s'},
        {name: 'modified', type: 'date', dateFormat: 'Y-m-d H:i:s'}
      ]);
      
      var reader = new Ext.data.JsonReader({
          totalProperty: 'total',
          root: 'rows',
          id: 'id'
      }, record);
      
      dataStore = new Ext.data.Store({
        proxy: new Ext.data.HttpProxy({
          url: Drupal.settings.alfresco.serviceGridUrl,
          method: 'GET'
        }),
        reader: reader,
        baseParams: {node: Drupal.settings.alfresco.folderRef},
        autoLoad: true,
        //remoteSort: true,
        //sortInfo: {field: 'name', direction: 'ASC'},
        listeners: {
          load: {
            fn: function(){
              Ext.getCmp('btn-download').disable();
              //Ext.getCmp('btn-delete').disable();
              Ext.getCmp('btn-open').disable();
              Ext.getCmp('btn-send').disable();
              Ext.getCmp('grid-details').disable();
            }
          }
        }
      });
      
      function renderName(value, p, record){
        var url = Drupal.settings.alfresco.iconsPath + '/' + record.data['icon'] + '.gif';
        return String.format('<span class="row-icon" style="background-image: url({1})" ext:qtip="{2}">{0}</span>', value, url, record.data['title']);
      }

      var columns = [
        {id: 'name', header: Drupal.t('Name'), dataIndex: 'name', sortable: false, width: 200, renderer: renderName},
        {header: Drupal.t('Size'), dataIndex: 'size', sortable: false, align: 'right', width: 80},
        {header: Drupal.t('Creator'), dataIndex: 'creator', sortable: false, width: 100},
        {header: Drupal.t('Date created'), dataIndex: 'created', width: 130, hidden: true, sortable: false, renderer: Ext.util.Format.dateRenderer('d-m-Y H:i:s')},
        {header: Drupal.t('Date modified'), dataIndex: 'modified', width: 130, sortable: false, renderer: Ext.util.Format.dateRenderer('d-m-Y H:i:s')},
        {header: Drupal.t('Title'), dataIndex: 'title', width: 200, sortable: false, hidden: true},
        {header: Drupal.t('Description'), dataIndex: 'description', width: 200, sortable: false, hidden: true},
        {header: Drupal.t('Author'), dataIndex: 'author', width: 100, sortable: false, hidden: true}
      ];
      
      var bar = new Ext.PagingToolbar({
        pageSize: Drupal.settings.alfresco.queryLimit,
        store: dataStore,
        displayInfo: true,
        autoWidth: true,
        displayMsg: Drupal.t('Displaying items {0} - {1} of {2}'),
        emptyMsg: Drupal.t('No items to display.'),

        // Ext JS 2.x
        onClick: function(which){
          if (which == "refresh") {
            var o = {}, pn = this.paramNames;
            o[pn.start] = this.cursor;
            o[pn.limit] = this.pageSize;
            o['cache'] = 'node';
            if (this.fireEvent('beforechange', this, o) !== false) {
              this.store.load({params:o});
            }
            return;
          }
          Ext.PagingToolbar.prototype.onClick.call(this, which);
        },
        
        // Ext JS 3.x
        doRefresh : function(){
          var o = {}, pn = this.getParams();
          o[pn.start] = this.cursor;
          o[pn.limit] = this.pageSize;
          o['cache'] = 'node';
          if(this.fireEvent('beforechange', this, o) !== false){
              this.store.load({params:o});
          }
        }
      });

      itemsGrid = new Ext.grid.GridPanel({
        id: 'items-grid',
        ds: dataStore,
        columns: columns,
        autoExpandColumn: 'name',
        bbar: bar,
        sm: new Ext.grid.RowSelectionModel({
          singleSelect:true,
          listeners: {
            rowselect: function(sm, row, rec) {
              Ext.getCmp('btn-download').enable();
              Ext.getCmp('btn-open').enable();
              
              if (Drupal.settings.alfresco.accessDelete) {
                Ext.getCmp('btn-delete').enable();
              }
              
              if (window.opener) {
                Ext.getCmp('btn-send').enable();
              }
              
              Ext.getCmp('grid-details').enable();
            },
            rowdeselect: function(sm, row, rec) {
            }
          }
        }),
        
        title: Drupal.settings.alfresco.folderName,
        region: 'center',
        loadMask: true,
        
        listeners: {
          'rowclick': function(grid, dataIndex) {
            var dataRow = dataStore.getAt(dataIndex);
            propsGrid.setSource(dataRow.data);
          },
          'rowdblclick': function(grid, dataIndex) {
            if (window.opener) {
              AlfrescoBrowser.SendItem(grid);
            } else {
              AlfrescoBrowser.DownloadItem(grid, false);
            }
        }},
        
        viewConfig: {
          getRowClass: function(record, index) {
            return record.get('nid').length > 0 ? 'row-node-exists' : 'row-node-new';
          }
        },

        tbar: [{
          id: 'btn-add',
          text: Drupal.t('Add'),
          tooltip: Drupal.t('Add content to this space.'),
          iconCls: 'upload',
          disabled: !Drupal.settings.alfresco.accessAdd,
          handler: function() {
            AlfrescoBrowser.AddItem(folderTree, dataStore);
          }
        }, {
          id: 'btn-add-folder',
          text: Drupal.t('Create Space'),
          tooltip: Drupal.t('Create Space.'),
          iconCls: 'create-space',
          disabled: !Drupal.settings.alfresco.accessAdd,
          handler: function() {
            AlfrescoBrowser.CreateSpace(folderTree);
          }
        }, {
          id: 'btn-delete',
          text: Drupal.t('Delete'),
          tooltip: Drupal.t('Delete selected item.'),
          iconCls: 'delete',
          disabled: true,
          handler: function() {
            AlfrescoBrowser.DeleteItem(itemsGrid, folderTree);
          }
        }, {
          id: 'btn-download',
          text: Drupal.t('Download'),
          tooltip: Drupal.t('Download selected item.'),
          iconCls: 'download',
          disabled: true,
          handler: function() {
            AlfrescoBrowser.DownloadItem(itemsGrid, true);
          }
        }, {
          id: 'btn-open',
          text: Drupal.t('View'),
          tooltip: Drupal.t('View selected item in new window.'),
          iconCls: 'view',
          disabled: true,
          handler: function() {
            AlfrescoBrowser.ViewItem(itemsGrid);
          }
        }, {
          id: 'btn-send',
          text: Drupal.t('Drupal'),
          tooltip: Drupal.t('Send selected item to Drupal.'),
          iconCls: 'drupal',
          disabled: true,
          handler: function() {
            AlfrescoBrowser.SendItem(itemsGrid);
          }
        }, {
          id: 'grid-details',
          text: Drupal.t('Properties'),
          tooltip: Drupal.t('View selected item properties.'),
          iconCls: 'details',
          disabled: true,
          //hidden: true,
          enableToggle: true,
          toggleHandler: function(item, pressed){
            if (pressed) {
              propsGrid.expand();
            } else {
              propsGrid.collapse();
            }
          }
        }],

        tools: [{
          id: 'refresh',
          qtip: Drupal.t('Clear content and search cache.'),
          on: {
            click: function(){
              var o = {start: 0, cache: 'all'};
              dataStore.load({params:o});
            }}
        }]
      });
      
      propsGrid = new Ext.grid.PropertyGrid({
        title: Drupal.t('Properties'),
        region: 'south',
        margins: '0 0 0 0',
        cmargins: '5 0 0 0',
        height: 150,
        minSize: 80,
        maxSize: 300,
        collapsible: true,
        collapsed: true,
        collapseMode: 'mini',
        split: true,
        hideCollapseTool: true,
        hideHeaders: true,
        listeners: {
        'validateedit': function(e) {
          e.cancel = true;
        },
        'expand': function(p) {
          Ext.getCmp('grid-details').toggle(true);
        },
        'collapse': function(p) {
          Ext.getCmp('grid-details').toggle(false);
        }}
      });
    }
  };
}();
Ext.onReady(AlfrescoBrowser.App.init, AlfrescoBrowser.App);