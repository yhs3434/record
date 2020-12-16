- drop down 만드는 법.

    ```html
    <ul>
        <li>1</li>
        <li class="dropdown">
            <a href="#" class="dropbtn">Dropdown</a>
            <div class="dropdown-content">
                <a href="#">Link 1</a>
                <a href="#">Link 2</a>
                <a href="#">Link 3</a>
            </div>
        </li>
    </ul>
    ```

    ```css
    li a, .dropbtn {
        display: inline-block;
    }

    li.dropdown {
        display: inline-block;
    }

    .dropdown-content {
        display: none;
        position: absolute;
    }

    .dropdown:hover .dropdown-content {
        display: block;
    }
    ```


다음 공부할 링크

https://www.w3schools.com/css/css_form.asp