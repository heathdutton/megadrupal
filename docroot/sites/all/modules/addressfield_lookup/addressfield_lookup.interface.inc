<?php

/**
 * @file
 * All interfaces required by the Address Field lookup module.
 */

/**
 * Interface that all Address Field lookup services need to implement.
 */
interface AddressFieldLookupInterface {

  /**
   * Set the term that we'll be using to lookup addresses.
   *
   * @param string $lookup_term
   *   String containing the lookup term.
   *
   * @return AddressFieldLookupInterface
   *   The called object.
   */
  public function setLookupTerm($lookup_term);

  /**
   * Perform the lookup based on the current term.
   *
   * @return bool
   *   Was the lookup successful or not?
   */
  public function lookup();

  /**
   * Get the results of the lookup.
   *
   * @return array $results
   *   Array of lookup results in the format:
   *     id - Address ID.
   *     street - Street (Address Line 1).
   *     place - Remainder of address.
   */
  public function getLookupResult();

  /**
   * Get the full details for a given address.
   *
   * @param mixed $address_id
   *   ID of the address to get details for.
   *
   * @return array $address
   *   Array of details for the given address in the format:
   *     id - Address ID
   *     sub_premise - The sub_premise of this address
   *     premise - The premise of this address. (i.e. Apartment / Suite number).
   *     thoroughfare - The thoroughfare of this address. (i.e. Street address).
   *     dependent_locality - The dependent locality of this address.
   *     locality - The locality of this address. (i.e. City).
   *     postal_code - The postal code of this address.
   *     administrative_area - The administrative area of this address.
   *     (i.e. State/Province)
   *     organisation_name - Contents of a primary OrganisationName element
   *     in the xNL XML.
   */
  public function getAddressDetails($address_id);

}
