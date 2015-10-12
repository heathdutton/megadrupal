<?php

/**
 * PostcodeAnywhere/Interactive/RetrieveById (v 1.30)
 * Returns the full address details based on the Id.
 * @see http://www.postcodeanywhere.co.uk/support/webservices/PostcodeAnywhere/Interactive/RetrieveById/v1.3/default.aspx
 */
class PostcodeAnywhere_Interactive_RetrieveById {

   //Includes country name based on postcode area and the thoroughfare name components
   //Credit: Thanks to Stuart Sillitoe (http://stu.so/me) for the original PHP that these samples are based on.

   private $Key; //The key to use to authenticate to the service.
   private $Id; //The Id from a Find method to retrieve the details for.
   private $PreferredLanguage; //The language version of the address to return.
   private $UserName; //The username associated with the Royal Mail license (not required for click licenses).
   private $Data; //Holds the results of the query
   private $Error; //Holds the error of the query if any.

   function PostcodeAnywhere_Interactive_RetrieveById($Key, $Id, $PreferredLanguage, $UserName)
   {
      $this->Key = $Key;
      $this->Id = $Id;
      $this->PreferredLanguage = $PreferredLanguage;
      $this->UserName = $UserName;
   }

   function MakeRequest()
   {
      $url = "http://services.postcodeanywhere.co.uk/PostcodeAnywhere/Interactive/RetrieveById/v1.30/xmla.ws?";
      $url .= "&Key=" . urlencode($this->Key);
      $url .= "&Id=" . urlencode($this->Id);
      $url .= "&PreferredLanguage=" . urlencode($this->PreferredLanguage);
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
             $this->Data[] = array('Udprn'=>$item->attributes()->Udprn,'Company'=>$item->attributes()->Company,'Department'=>$item->attributes()->Department,'Line1'=>$item->attributes()->Line1,'Line2'=>$item->attributes()->Line2,'Line3'=>$item->attributes()->Line3,'Line4'=>$item->attributes()->Line4,'Line5'=>$item->attributes()->Line5,'PostTown'=>$item->attributes()->PostTown,'County'=>$item->attributes()->County,'Postcode'=>$item->attributes()->Postcode,'Mailsort'=>$item->attributes()->Mailsort,'Barcode'=>$item->attributes()->Barcode,'Type'=>$item->attributes()->Type,'DeliveryPointSuffix'=>$item->attributes()->DeliveryPointSuffix,'SubBuilding'=>$item->attributes()->SubBuilding,'BuildingName'=>$item->attributes()->BuildingName,'BuildingNumber'=>$item->attributes()->BuildingNumber,'PrimaryStreet'=>$item->attributes()->PrimaryStreet,'SecondaryStreet'=>$item->attributes()->SecondaryStreet,'DoubleDependentLocality'=>$item->attributes()->DoubleDependentLocality,'DependentLocality'=>$item->attributes()->DependentLocality,'PoBox'=>$item->attributes()->PoBox,'PrimaryStreetName'=>$item->attributes()->PrimaryStreetName,'PrimaryStreetType'=>$item->attributes()->PrimaryStreetType,'SecondaryStreetName'=>$item->attributes()->SecondaryStreetName,'SecondaryStreetType'=>$item->attributes()->SecondaryStreetType,'CountryName'=>$item->attributes()->CountryName,'CountryISO2'=>$item->attributes()->CountryISO2,'CountryISO3'=>$item->attributes()->CountryISO3);
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
