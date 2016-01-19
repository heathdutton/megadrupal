/**
 * Creates an Ext JS Grid panel to list Drupal node details.
 */
Ext.define('DrupalExt.view.node.Grid', {
  extend: 'Ext.grid.Panel',
  alias: 'widget.nodeGrid',
  store: 'Node',
  columns: [
    { header: 'NID',  dataIndex: 'nid' },
    { header: 'VID',  dataIndex: 'vid' },
    { header: 'Title',  dataIndex: 'title' },
    { header: 'Type', dataIndex: 'type' },
    { header: 'Status', dataIndex: 'status' }
  ],
  
  initComponent: function() {
    this.callParent(arguments);
    
    this.on('render', function() {
      this.getStore().load();
    });
  }
});