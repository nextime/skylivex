function changeContent(id, content)
{
   var n=document.getElementById(id);
   if(typeof(n)!="undefined")
     n.innerHTML=content;
}
function notify(content)
{
   if(typeof(notifycb)=="function")
      notifycb(content);
}
function msgalert(content)
{
   if(typeof(alertcb)=="function")
      alertcb(content);
   else
      alert(content);
}
function publicReceived(user, msg)
{
   if(typeof(public_received)=="function")
   {
      public_received(user, msg);
   } else {
      n = document.getElementById("chatarea");
      if(typeof(n)!="undefined")
      {
         var newel = document.createElement('p');
         newel.innerHTML="&lt;"+user+"&gt; "+msg;
         n.appendChild(newel);
         n.scollTop = n.scrollHeight;
      }
   }
}

SkyliveX.changeContent.connect(changeContent);
SkyliveX.notify.connect(notify);
SkyliveX.alertmsg.connect(msgalert);
SkyliveX.public_received.connect(publicReceived);
