<?php

/**
 * @file
 * Copyright (C) 2012, Victor Nikulshin
 *
 * This file is part of KKB Epay.
 *
 * KKB Epay is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * KKB Epay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KKB Epay.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Private key loaders' interface.
 */
interface KkbEpay_KeyLoaderInterface {

  /**
   * Must return an instance of the KkbEpay_Key class.
   *
   * @return KkbEpay_Key|NULL
   *   If key was successfully loaded, an instance of KkbEpay_Key class with
   *   all required information about the key.
   *   NULL in case of any error, e.g. key was not found.
   */
  public function getKey();

  /**
   * Must set debugging mode.
   *
   * @param boolean $flag
   *   TRUE, if payment debugging is performed. In this case, the loader must
   *         load a special debugging key;
   *   FALSE, if real payment is performed. In this case, the real private key
   *          must be loaded.
   *
   * @return NULL
   *   Nothing.
   */
  public function setDebug($flag);

  /**
   * Must check that the private key can be loaded and is valid.
   *
   * @return boolean
   *   TRUE if private key is fine;
   *   FALSE if there are any problems with the key.
   */
  public function validateKey();

}
