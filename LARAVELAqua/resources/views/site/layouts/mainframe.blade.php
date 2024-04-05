<!DOCTYPE html>
<html lang="pt-br">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="{{ asset('css/index.css')}}">

    <link href="//cdn.jsdelivr.net/npm/round-slider@1.5.1/dist/roundslider.min.css" rel="stylesheet" />
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.1/css/all.min.css"
        integrity="sha512-DTOQO9RWCH3ppGqcWaEA1BIZOC6xxalwEsw9c2QQeAIftl+Vegovlnee1c9QX4TctnWMn13TZye+giMm8e2LwA=="
        crossorigin="anonymous" referrerpolicy="no-referrer" />

    <title>@yield('title')</title>
</head>


<body>

@include('site._partials.topo')
@yield('content')
@include('site._partials.footer')



</body>



<script src="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.1/js/all.min.js"
    integrity="sha512-GWzVrcGlo0TxTRvz9ttioyYJ+Wwk9Ck0G81D+eO63BaqHaJ3YZX9wuqjwgfcV/MrB2PhaVX9DkYVhbFpStnqpQ=="
    crossorigin="anonymous" referrerpolicy="no-referrer"></script>


<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>

<script src="//code.jquery.com/jquery-3.2.1.js"></script>

<script src="https://cdn.jsdelivr.net/npm/round-slider@1.5.1/dist/roundslider.min.js"></script>

<script src="{{ asset('js/index.js')}}"></script>

</html>
