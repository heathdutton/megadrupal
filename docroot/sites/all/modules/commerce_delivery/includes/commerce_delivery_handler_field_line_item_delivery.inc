<?php

/**
 * @file
 * Contains the basic delivery field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to an order.
 */
class commerce_delivery_handler_field_line_item_delivery extends views_handler_field {
	function query() {
		$result = db_query("SELECT li.delivery_ref_value FROM {field_data_delivery_ref} li WHERE li.line_item_id IN (:line_item_ids)", array(':line_item_ids' => $this->value));
		foreach ($result as $line_item) {
			$values[] = intval($line_item->delivery_ref_value);
		}
	}
}
