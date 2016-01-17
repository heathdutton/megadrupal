<?php

/**
 * Null implementation of the mail backend.
 */
class DevControl_NullMailSystem implements MailSystemInterface
{
   public function format(array $message)
   {
       return array();
   }

   public function mail(array $message)
   {
       return true;
   }
}
