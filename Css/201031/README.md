# 2020년 10월 31일

clearfix를 왜 쓰는가. = 버그 없애려고.

a:link
a:visited
a:hover
a:active

이 순서로 수도 코드를 쓰는게 가장 효과적.


p {
  display: none;
  background-color: yellow;
  padding: 20px;
}

div:hover p {
  display: block;
}

라고 하면 div hover일 때 p가 나타남.

다음 공부할 것 : https://www.w3schools.com/css/css_pseudo_elements.asp ~