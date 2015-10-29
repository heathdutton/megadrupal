Ext.onReady(function() {
  
  /*
  var store = Ext.data.StoreManager.lookup('NodePage');
  
  store.filters.add(new Ext.util.Filter({
    property : 'body',
    value: 'tn',
    anyMatch: 1,
    caseSensitive: 0
  }));
  store.filters.add(new Ext.util.Filter({
    property : 'status',
    value: true
  }));
  store.filters.add(new Ext.util.Filter({
    property : 'field_page_parent_nid',
    value: 1
  }));
  
  store.sorters.add(new Ext.util.Sorter({
    property : 'created',
  }));
  store.sorters.add(new Ext.util.Sorter({
    property : 'title',
    direction: 'DESC'
  }));
  
  store.load({
    scope   : this,
    //callback: function(records, operation, success) {
    //  console.log(records);
    //}
  });
  console.log(store);
  //*/
  
  
  /*var store = Ext.data.StoreManager.lookup('TreeStore1');
  store.load({
    scope   : this,
    //callback: function(records, operation, success) {
    //  console.log(records);
    //}
  });
  console.log(store);
  // */
  
  //Load a store from a View.
  /*var store = Ext.data.StoreManager.lookup('ViewUsersVDNDefault');
  store.load({
    scope   : this,
    callback: function(records, operation, success) {
      console.log(records);
    }
  });
  // */  
  
  //Load a node of type page from the server/drupal (nid: 1)
  /*DrupalExt.model.NodePage.load(1, {
    failure: function(record, operation) {
        console.log("failed to load node");
    },
    success: function(record, operation) {
      console.log('node', record);
      
      // Load the user associated with this node.
      record.getUidUser(function(user, operation) {
        console.log('user', user);
      }, this);
      
      //console.log('assoc list', record.noderefNidNodePageList().getRange())
      
      
      //record2 = record.copy();
      //Ext.data.Model.id(record2); //Give record a new ID (so we don't update existing record).
      
      // Set a regular field (single value)
      //record2.set('title', 'title set by ext ' + Math.random());
      
      //Set a CCK field (is an array).
      //var field_test = record.get('field_test');
      //field_test[0].value = 'blah';
      //record.set('field_test',field_test);
      
      // Save new record back to server/drupal
      //record2.save();
      
      // Should see new nid.
      //console.log(record2.data);
    },
    callback: function(record, operation) {
        //do something whether the load succeeded or failed
    }
  });
  // */
});
