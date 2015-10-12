<?php
/**
 * @file
 * Chargebee Event API.
 */

function chargebee_event_deserialize($content) {
  $deserialize = ChargeBee_Event::deserialize($content);
  return $deserialize;
}
