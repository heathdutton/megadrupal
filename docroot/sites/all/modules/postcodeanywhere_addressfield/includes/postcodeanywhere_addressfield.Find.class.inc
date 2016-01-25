<?php

/**
 * PostcodeAnywhere/Interactive/Find (v 1.10)
 * Lists address records matching the specified search term.
 * This general search method can search by postcode, company or street.
 * @see http://www.postcodeanywhere.co.uk/support/webservices/PostcodeAnywhere/Interactive/Find/v1.1/default.aspx
 */
class PostcodeAnywhere_Interactive_Find {

   //Includes support for Welsh alternative addresses when searching by postcode and output filtering
   //Credit: Thanks to Stuart Sillitoe (http://stu.so/me) for the original PHP that these samples are based on.

   private $Key; //The key to use to authenticate to the service.
   private $SearchTerm; //The search term to find. The search term can be a postcode, company name or street and town (separated by commas).
   private $PreferredLanguage; //The language version of the address to return.
   private $Filter; //The filter to apply to the output.
   private $UserName; //The username associated with the Royal Mail license (not required for click licenses).
   private $Data; //Holds the results of the query.
   private $Error; //Holds the error of the query if any.

   function PostcodeAnywhere_Interactive_Find($Key, $SearchTerm, $PreferredLanguage, $Filter, $UserName)
   {
      $this->Key = $Key;
      $this->SearchTerm = $SearchTerm;
      $this->PreferredLanguage = $PreferredLanguage;
      $this->Filter = $Filter;
      $this->UserName = $UserName;
   }

   function MakeRequest()
   {
      $url = "http://services.postcodeanywhere.co.uk/PostcodeAnywhere/Interactive/Find/v1.10/xmla.ws?";
      $url .= "&Key=" . urlencode($this->Key);
      $url .= "&SearchTerm=" . urlencode($this->SearchTerm);
      $url .= "&PreferredLanguage=" . urlencode($this->PreferredLanguage);
      $url .= "&Filter=" . urlencode($this->Filter);
      $url .= "&UserName=" . urlencode($this->UserName);

      //Make the request to Postcode Anywhere and parse the XML returned
      $file = simplexml_load_file($url);

      //Check for an error, if there is one then throw an exception
      if ($file->Columns->Column->attributes()->Name == "Error") 
      {
        foreach ($file->Rows->Row->attributes() as $key => $value) {
          $this->Error[$key]= (string)$value;
        }
        return;
      }

      //Copy the data
      if ( !empty($file->Rows) )
      {
         foreach ($file->Rows->Row as $item)
         {
             $this->Data[] = array('Id'=>$item->attributes()->Id,'StreetAddress'=>$item->attributes()->StreetAddress,'Place'=>$item->attributes()->Place);
         }
      }
   }

   function HasData()
   {
      if ( !empty($this->Data) )
      {
         return $this->Data;
      }
      return false;
   }

   function HasError()
   {
      if ( !empty($this->Error) )
      {
         return $this->Error;
      }
      return false;
   }

}
