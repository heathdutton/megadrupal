<?php

/**
 * Field handler to provide a count of patches applied to package items.
 */
class project_package_handler_field_local_patch_count extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['package_nid'] = 'package_nid';
  }

  function pre_render(&$values) {
    $this->items = array();

    // Normally, we'd need to do a separate query for every unique pair of
    // package_nid and item_nid. However, we can combine queries if either of
    // those is shared by multiple pairs. In the common case, one of them is
    // going to be fixed across all results in the view and only the other
    // will vary. In that case, we can find everything we need in a single
    // query. So, we keep track of both all items for a given package, and all
    // packages for a given item. Once we've got all of that, we see which
    // table is smaller, since that's the one that will require the fewest
    // queries.
    $items_by_package = array();
    $packages_by_item = array();
    foreach ($values as $result) {
      $item_nid = $result->{$this->field_alias};
      $package_nid = $result->{$this->aliases['package_nid']};
      $items_by_package[$package_nid][] = $item_nid;
      $packages_by_item[$item_nid][] = $package_nid;
    }

    // We have less buckets in the items_by_package table, meaning it was
    // mostly results across many items from the same package.
    if (count($items_by_package) < count($packages_by_item)) {
      foreach ($items_by_package as $package_nid => $item_nids) {
        $result = db_query("SELECT COUNT(patch_file_url) AS count, package_nid, item_nid FROM {project_package_local_patch} WHERE package_nid = :package_nid AND item_nid IN (:item_nids) GROUP BY item_nid", array(':package_nid' => $package_nid, ':item_nids' => $item_nids));
        foreach ($result as $data) {
          $this->items[$data->package_nid][$data->item_nid] = $data->count;
        }
      }
    }
    // Less buckets in the packages_by_item table, meaning it was mostly 
    // results across multiple packages for the same item.
    else {
      foreach ($packages_by_item as $item_nid => $package_nids) {
        $result = db_query("SELECT COUNT(patch_file_url) AS count, package_nid, item_nid FROM {project_package_local_patch} WHERE item_nid = :item_nid AND package_nid IN (:package_nids) GROUP BY package_nid", array(':item_nid' => $item_nid, ':package_nids' => $package_nids));
        foreach ($result as $data) {
          $this->items[$data->package_nid][$data->item_nid] = $data->count;
        }
      }
    }
  }

  function render($values) {
    $item_nid = $values->{$this->field_alias};
    $package_nid = $values->{$this->aliases['package_nid']};
    if (!empty($this->items[$package_nid][$item_nid])) {
      return format_plural($this->items[$package_nid][$item_nid], '1 patch', '@count patches');
    }
    return '';
  }
}
