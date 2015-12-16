
FileManager = {
	expanded: new Array()
};

FileManager.toggle = function(row) {

	var children = jQuery(row).next();
	
	var id = jQuery(row).attr('id');
	var form = jQuery('#file-maintenance-browser-form');
	
	if (children.hasClass('expanded')) {
		/*
		 * Collapse
		 */
	  jQuery(row).find('.toggle-element img.plus').show();
		jQuery(row).find('.toggle-element img.minus').hide();
    
		children.removeClass('expanded');
    delete FileManager.expanded[id];

    // TODO
    jQuery.each(children.find('.tree-children'), function(index, child) {
      jQuery(child).removeClass('expanded');
      var prev = jQuery(child).prev();
      delete FileManager.expanded[prev.attr('id')];
    });
    
		children.find('img.plus').show();
		children.find('img.minus').hide();
	} else {
	  /*
	   * Expand
	   */
	  children.addClass('expanded');
	  FileManager.expanded[id] = true;
		
		jQuery(row).find('.toggle-element img.plus').hide();
		jQuery(row).find('.toggle-element img.minus').show();
	}
	
//	jQuery("#edit-expanded").val(jQuery(FileManager.expanded).serializeArray()); // expanded.join(',')
	// TODO This surely can be done better. Move to ajax handler.
	var keys = [];
	for (var k in FileManager.expanded) keys.push(k);
	jQuery("#expanded-list").val(keys.join(','));
	
}

FileManager.updateTableHeader = function(table) {

	FileManager.originalDataCells.each(function (index) {
        jQuery(this).css('width', '');
	});
	
	FileManager.originalHeader.show();

	originalWidth = (parseInt(FileManager.originalTable.css('width')) + 1) + 'px';

	FileManager.stickyHeaderCells.each(function (index) {
    var add = index == 0 ? 1 : 0;
    var cellWidth = jQuery(FileManager.originalDataCells.get(index)).css('width');

    // Exception for IE7.
    if (cellWidth == 'auto') {
      cellWidth = FileManager.originalDataCells.get(index).clientWidth + 'px';
    }
    jQuery(this).css('width', cellWidth);
    // TODO
    jQuery(FileManager.originalDataCells.get(index)).css('width', cellWidth);
  });

	FileManager.stickyTable.css('width', originalWidth);
	FileManager.originalHeader.hide();

}


/**
 * Attaches sticky table headers.
 */
Drupal.behaviors.staticHeader = {
	attach: function (context, settings) {
//    if (!$.support.positionFixed) {
//      return;
//    }

//    	jQuery('table.static-header', context).ready(function () {
	jQuery('table.static-header', context).once('static-header', function () {
    	//alert('Y');
    	FileManager.tableHeader(jQuery('table.static-header'));
//      $(this).data("drupal-tableheader", new Drupal.tableHeader(this));
    });
  }
};

/**
 * Attaches resizable.
 */
Drupal.behaviors.resizableBrowser = {
  attach: function (context, settings) {
    jQuery('#directory-tree-wrapper').once('resizable', function () {
      jQuery('#directory-tree-wrapper').resizable({ handles: 'e' });
      jQuery('#directory-tree-wrapper').resize(FileManager.updateTableHeader);
    });
    
    jQuery("#file-maintenance-browser-form").submit(function () {
      console.log("submit");
    });
  }
};

/**
 *
 * @param table
 */
FileManager.tableHeader = function(table) {
  var self = this;

  this.originalTable = jQuery(table);
  this.originalHeader = jQuery(table).children('thead');
  this.originalHeaderCells = this.originalHeader.find('> tr > th');

  // TODO
  var row = jQuery(table).children('tbody').children('tr').first();
  this.originalDataCells = row.children('td');

  // TODO one pixel is additional 1 px header border, the other is 1px additional 
  originalWidth = (parseInt(this.originalTable.css('width')) + 1) + 'px';

  this.stickyTable = jQuery('<table class="file-list-table sticky-header-table"/>')
    //.insertBefore('#file-list-table-scrollview')
    .appendTo('#file-list-table-header')
  
    //.css({ position: 'fixed', top: '0px' });
  this.stickyHeader = this.originalHeader.clone(true)
    //.hide()
    .appendTo(this.stickyTable);
  this.stickyHeaderCells = this.stickyHeader.find('> tr > th');

  // We hid the header to avoid it showing up erroneously on page load;
  // we need to unhide it now so that it will show up when expected.
  this.stickyHeader.show();
  
  //this.eventhandlerRecalculateStickyHeader(originalWidth);
  FileManager.updateTableHeader();
  
//  this.originalHeader.hide();
}

