<div class="header">
    <div class="header-title">
        <h3>Aqua Manager</h3>
        <h5>@yield('title')</h5>
    </div>
</div>
<div class="floating-menu">
    <div class="floating-body">
        <div class="floating-optins">
            <ul>
                <li><a href="{{route('home')}}"><i class="fas fa-chart-line"></i><span>Leituras</span></a></li>
                <li><a href="{{route('settings')}}"><i class="fas fa-gear"></i><span>Ajustes</span></a></li>
            </ul>
        </div>
        <div class="trigger" onclick="toggleFloating(this)">
            <i class="fas fa-bars"></i>
        </div>
    </div>
</div>