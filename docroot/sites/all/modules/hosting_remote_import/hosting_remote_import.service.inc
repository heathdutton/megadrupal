<?php

/**
 * The base service type class , registered with hook_hosting_service_type.
 */
class hostingService_remote_import extends hostingService {
  /**
   *   the value stored in the service column of hosting_service table.
   */
  public $service = 'remote_import';
}


/**
 * An implementation of the example service type, registered with hook_hosting_service.
 */
class hostingService_remote_import_hostmaster extends hostingService_remote_import {
  /**
   *   the value stored in the type column of the hosting_service table.
   */
  public $type = 'hostmaster';


  /**
   *   this service needs to have a port specified for it.
   */
  public $has_port = FALSE;

}

