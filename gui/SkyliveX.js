function changeContent(id, content)
{
   document.getElementById(id).innerHTML=content;
}

SkyliveX.changeContent.connect(changeContent);
