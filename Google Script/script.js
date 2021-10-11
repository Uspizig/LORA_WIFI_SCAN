
//https://www.thethingsindustries.com/docs/getting-started/migrating/major-changes/
function test(e){
  var sheet = SpreadsheetApp.openById("FILLMEIN");
  var firstSheet = sheet.getSheets()[0];
  sheet.setName("TTGOTDatenspeicher");
}


function doPost(e) {
  
  Logger.log("I was called")
  if(typeof e !== 'undefined')
  Logger.log(e.parameter);
  
  
  
  var sheet = SpreadsheetApp.openById("FILLMEIN");
  var firstSheet = sheet.getSheets()[0];
  firstSheet.setName("Lora-Datenspeicher");
  //1. Zeile
  firstSheet.getRange(1, 1).setValue('New RAW-Data Posted from TTNetwork:');
  firstSheet.getRange(1, 2).setValue(JSON.stringify(e));
  firstSheet.getRange(2,1).setValue(['Timestamp']);
  firstSheet.getRange(2,2).setValue(['RAW']);
  firstSheet.getRange(2,3).setValue(['Port:']); 
  firstSheet.getRange(2,4).setValue(['Packet#']);
  firstSheet.getRange(2,5).setValue(['RSSI']);
  firstSheet.getRange(2,6).setValue(['SNR']);
  firstSheet.getRange(2,7).setValue(['MAC']);
  firstSheet.getRange(2,8).setValue(['PEGEL']);
  firstSheet.getRange(2,9).setValue(['MAC2:']);
  firstSheet.getRange(2,10).setValue(['PEGEL2:']);
  firstSheet.getRange(2,11).setValue(['MAC3:']);
  firstSheet.getRange(2,12).setValue(['PEGEL3:']);
  firstSheet.getRange(2,13).setValue(['LAT:']);
  firstSheet.getRange(2,14).setValue(['LON:']);
  firstSheet.getRange(2,15).setValue(['GENAUIGKEIT:']);
  var rawdata = e;
  
  // 2. Zeile
  //firstSheet.appendRow([rawdata]);
  var text = JSON.stringify(e);
  if (text) {
    if (text.indexOf('contents') > -1) {
        var textcutted = text.slice(text.indexOf('contents')+11, text.length); // cut everything useless to get only the payload data
        //firstSheet.appendRow(['textcutted' , textcutted]);
        
        textcutted = textcutted.replace(/\\/g, "");//remove all "\" to generate a proper Json that can be converted
        //firstSheet.appendRow(['textcutted' , textcutted]);
      //for simulated Data
      //var textcutted2 = textcutted.slice(1, textcutted.indexOf('postData')-10); // remove last letters to get a Json that can be converted
      
      //for real Data
      var textcutted2 = textcutted.slice(0, textcutted.indexOf('postData')-24); // remove last letters to get a Json that can be converted
      var endetest = textcutted2.slice(textcutted2.length -10,textcutted2.length);//just for debug: Shows end of Payload
      //var anfangtest = textcutted2.slice(0,10);//just for debug: Shows begin of Payload
      //firstSheet.appendRow(['textcutted2' , textcutted2]);  //just for debug
    }
    else{
      firstSheet.appendRow(['there was' , 'no content', 'that could', 'be extracted']);//in case something went wrong
      firstSheet.appendRow(['Text does not contain contents:' , text]);//print text in case something went wrong
    }
  }
  else{
   firstSheet.appendRow(['Text that has been posted is empty:' , text]);//print text in case something went wrong
  }
  
  var jsonData = JSON.parse(textcutted2); // parse the extracted Text in JSON Converter
  if (jsonData.uplink_message.f_port < 5){//v3
    
    
    
    // TTN V3 STACK
    var mac = jsonData.uplink_message.decoded_payload.BYTE1 + ':' + jsonData.uplink_message.decoded_payload.BYTE1 + ':' + jsonData.uplink_message.decoded_payload.BYTE3 + ':' + jsonData.uplink_message.decoded_payload.BYTE4 + ':' + jsonData.uplink_message.decoded_payload.BYTE5 + ':' + jsonData.uplink_message.decoded_payload.BYTE6;
    var mac2 = jsonData.uplink_message.decoded_payload.BYTE8 + ':' + jsonData.uplink_message.decoded_payload.BYTE9 + ':' + jsonData.uplink_message.decoded_payload.BYTE10 + ':' + jsonData.uplink_message.decoded_payload.BYTE11 + ':' + jsonData.uplink_message.decoded_payload.BYTE12 + ':' + jsonData.uplink_message.decoded_payload.BYTE13;
    var mac3 = jsonData.uplink_message.decoded_payload.BYTE15 + ':' + jsonData.uplink_message.decoded_payload.BYTE16 + ':' + jsonData.uplink_message.decoded_payload.BYTE17 + ':' + jsonData.uplink_message.decoded_payload.BYTE18 + ':' + jsonData.uplink_message.decoded_payload.BYTE19 + ':' + jsonData.uplink_message.decoded_payload.BYTE20;
    var pegel = parseInt(jsonData.uplink_message.decoded_payload.BYTE7, 16)-140;
    var pegel2 = parseInt(jsonData.uplink_message.decoded_payload.BYTE14, 16)-140;
    var pegel3 = parseInt(jsonData.uplink_message.decoded_payload.BYTE21, 16)-140;
    var position = geo(mac, mac2, mac3,  pegel, pegel2, pegel3);
    //firstSheet.appendRow(['Position', position]);
    
    /*//to detect if Google Geolocation API was working correct
    if(position.length >3){
      firstSheet.appendRow([jsonData.received_at, jsonData.uplink_message.frm_payload, jsonData.uplink_message.f_port, jsonData.uplink_message.f_cnt, jsonData.uplink_message.rx_metadata[0].rssi, jsonData.uplink_message.rx_metadata[0].snr, mac, pegel, mac2, pegel2, mac3, pegel3, position]);
    }
    else{
      firstSheet.appendRow([jsonData.received_at, jsonData.uplink_message.frm_payload, jsonData.uplink_message.f_port, jsonData.uplink_message.f_cnt, jsonData.uplink_message.rx_metadata[0].rssi, jsonData.uplink_message.rx_metadata[0].snr, mac, pegel, mac2, pegel2, mac3, pegel3]);
    }*/
    
    
	//staight forward usecase
	firstSheet.appendRow([jsonData.received_at, jsonData.uplink_message.frm_payload, jsonData.uplink_message.f_port, jsonData.uplink_message.f_cnt, jsonData.uplink_message.rx_metadata[0].rssi, jsonData.uplink_message.rx_metadata[0].snr, mac, pegel, mac2, pegel2, mac3, pegel3, position]);
    //var lati =  position.slice(0, position.indexOf(':'));
    //var rest = position.slice(position.indexOf(':')+1, position.length); 
    //var lon =  rest.slice(0, rest.indexOf(':')); 
    //var genauigkeit =  rest.slice(rest.indexOf(':')+1, rest.length); 
    //firstSheet.appendRow(['Lat', lati]);
  }
  else {
   firstSheet.appendRow(['0' ,'0','0','NO BME Data sent']);  
  }
  
  
  
  return ContentService.createTextOutput(JSON.stringify(e))
}

function doGet(e){
  test(e);
}


function geo(mac, mac2, mac3, pegel, pegel2, pegel3){
  var test = '{"wifiAccessPoints":[{"macAddress":"XX:XX:AB:71:A8:90","signalStrength":-40,"channel":9},{"macAddress":"XX:XX:6F:77:43:90","signalStrength":-81,"channel":6},{"macAddress":"XX:XX:05:C2:A5:09","signalStrength":-78,"channel":11},{"macAddress":"XX:XX:20:D7:7D:54","signalStrength":-67,"channel":11},{"macAddress":"8A:8A:20:D7:7D:54","signalStrength":-66,"channel":11},{"macAddress":"7A:8A:20:D7:7D:54","signalStrength":-64,"channel":11}]}';
  var apiKey ='FILLMEIN';  
  var url = 'https://www.googleapis.com/geolocation/v1/geolocate?key=FILLMEIN'
  var urlfinished = url;
  
  //console.log(mac);
  //console.log(mac2);
  //console.log(test);
  
  var anfrage = '{"wifiAccessPoints":[{"macAddress":"';
  anfrage+=mac;
  anfrage+='","signalStrength":';
  anfrage+=pegel;
  anfrage+='},{"macAddress":"';
  anfrage+=mac2;
  anfrage+='","signalStrength":';
  anfrage+=pegel2;
  anfrage+='},{"macAddress":"';
  anfrage+=mac3;
  anfrage+='","signalStrength":';
  anfrage+=pegel3;
  anfrage+='}]}';
  
  var options = {
  'method' : 'post',
  'validateHttpsCertificates' : true,
  'followRedirects': true, 
  'muteHttpExceptions': false,  
  'contentType': 'application/json',
  'payload' : anfrage
  };
  
  var response2 = UrlFetchApp.fetch(urlfinished, options);
  var content_lantwort = response2.getContentText();
  var content_response_code = response2.getResponseCode();
  console.log(response2);
  console.log(content_lantwort);
  console.log(content_response_code);
  var antwort = JSON.parse(content_lantwort);
  var debug = antwort;
  var lat = antwort.location.lat;
  var lon = antwort.location.lng;
  var accuracy = antwort.accuracy;
  console.log(lat);
  console.log(lon);
  console.log(accuracy);
  console.log(anfrage);
  var answer = lat;
  answer += ':';
  answer += lon;
  answer += ':'
  answer += accuracy;
  return answer;
}



function myResponse() {
  
  
  var data = {
    "dev_id": "0123456",
    "port": 1,
    "confirmed": true,
    //"payload_raw" : "1QIDBA=="
    "payload_fields": {    // The JSON object to be encoded by your encoder payload function
      "temp_aktuell": 35, 
      "humidity": 20,  
      "temp_max": 50, 
      "temp_min": -20, 
      "condition_code": 500, 
      "muell": 1
    }
  };
  // Payload: Temp[1B] / Humidity [1B] / Temp_max [1B] /Temp_min [1B] / Condition_Code[2B] / Muell[1B] 
  // A2 93 B1 6B F4 01 80 das war der Inhalt
  /* Temp_aktuell:10
  Feuchte:50
  Temp_max:35
  Temp_min:-20
  Condition_Code:500
  Muell:1
  */

  var options =
   {
     'method' : 'post',
     'contentType': 'application/json',
     'payload': JSON.stringify(data)
   };
  
  var response = UrlFetchApp.fetch("FILLMEIN", options);  
  //var response = UrlFetchApp.fetch('http://www.google.com/');
  Logger.log(response.getContentText()); 
  var Test = "0"  
}



