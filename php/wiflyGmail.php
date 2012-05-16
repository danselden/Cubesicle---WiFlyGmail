<?php

$mailbox = '{imap.gmail.com:993/ssl/novalidate-cert}INBOX';
$username = $_GET['username'];
$password = $_GET['password'];
$msg = $_GET['msg'];


$mbox = imap_open($mailbox, $username, $password);
$n = imap_num_msg($mbox);


// define a variable for the data file
$myFile = "gmailCount.txt";

//$imap_obj = imap_check($mbox);
//var_dump($imap_obj);

//echo "<br><hr>";
//echo $n;
//echo " total messages.";
//echo "<br><hr>";
//$body = imap_body($mbox, $msg);

$unread = 0;
for ($i = $n; $i >= $n-70; $i--) {      
      $header = imap_headerinfo($mbox, $i);
	  if ($header->Unseen == "U"){
		$unread ++;
	  }
}
$fh = fopen($myFile, 'r') or die("can't open file");
// read the last count from the data file
$lastCount = fread($fh, filesize($myFile));
//close
fclose($fh);
// check to see if new count is larger
if($unread>$lastCount){
	echo '#';
	//and replace lastCount with new count
	$fh = fopen($myFile, 'w') or die("can't open file");
	fwrite($fh, $unread);
	fclose($fh);
}
// check to see if new count is the same… and tell the arduino !
else if($unread == $lastCount){
	echo '!';
}
// if for some reason the count is less, reset the count in preparation of the next read
else if($unread < $lastCount){
	echo '!';
	$fh = fopen($myFile, 'w') or die("can't open file");
	fwrite($fh, $unread);
	fclose($fh);
}

?>