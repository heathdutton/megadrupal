Array.prototype.containsNot = function(obj) {
  var i = this.length;
  while (i--) {
    if (this[i] == obj) {
      return false;
    }
  }
  return true;
}
function addNidToExcludeList(nid) {
  var filtered_nids_list = Array();  
  if(!isNaN(parseFloat(nid)) && isFinite(nid) && (nid != 0)){
     if (!(isNaN(nid)) ) {
      filtered_nids_list = getFilteredNidList();
      if (filtered_nids_list.containsNot(nid)) {
        filtered_nids_list.push(nid);
      }
      jQuery('#edit-search-exclude-nid-search-exclusion-nids').val(filtered_nids_list.join(','));
    }
    filtered_nids_list = getFilteredNidList();
    jQuery('#edit-search-exclude-nid-search-exclusion-nids').val(filtered_nids_list.join(','));  
  }
}
function getFilteredNidList() {
  var nidsList = jQuery('#edit-search-exclude-nid-search-exclusion-nids').val().split(',');
  var filtered_nids_list = new Array();
  var intRegex = /^\d+$/;
  for (var i in nidsList) {
    if (intRegex.test(nidsList[i]) && (nidsList[i] > 0) && (filtered_nids_list.containsNot(nidsList[i]))) {
      filtered_nids_list.push(nidsList[i]);
    }
  }
  return filtered_nids_list;
}
