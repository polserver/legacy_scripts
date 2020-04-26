use os;
use uo;

Function CheckCity(item)
  If ( item.x>=1244 and item.y>=1530 and item.x <= 1704 and item.y <= 1758)
      Return 1;
    
    Elseif ( item.x>=4379 and item.y>=1045 and item.x <= 4487 and item.y <= 1178)
      Return 1;
      
    Elseif ( item.x>=1294 and item.y>=3664 and item.x <= 1506 and item.y <= 3886)
      Return 1;
      
    Elseif ( item.x>=494 and item.y>=926 and item.x <= 600 and item.y <= 1034)
      Return 1;
      
    Elseif ( item.x>=590 and item.y>=796 and item.x <= 670 and item.y <= 886)
      Return 1;
      
    Elseif ( item.x>=2394 and item.y>=370 and item.x <= 2624 and item.y <= 632)
      Return 1;

    Elseif ( item.x>=1794 and item.y>=2638 and item.x <= 2136 and item.y <= 2902)
      Return 1;

    Elseif ( item.x>=538 and item.y>=2106 and item.x <= 686 and item.y <= 2274)
      Return 1;

    Elseif ( item.x>=3646 and item.y>=2040 and item.x <= 3814 and item.y <= 2280)
      Return 1;

    Elseif ( item.x>=3588 and item.y>=2446 and item.x <= 3762 and item.y <= 2690)
      Return 1;

    Elseif ( item.x>=2612 and item.y>=2082 and item.x <= 2772 and item.y <= 2256)
      Return 1;

    Elseif ( item.x>=3496 and item.y>=1062 and item.x <= 3808 and item.y <= 1400)
      Return 1;

    Elseif ( item.x>=2794 and item.y>=634 and item.x <= 3026 and item.y <= 1000)
      Return 1;

    Elseif ( item.x>=2206 and item.y>=1116 and item.x <= 2290 and item.y <= 1236)
      Return 1;

    Elseif ( item.x>=5120 and item.y>=0 and item.x <= 6128 and item.y <= 1992)
      Return 1;

    Elseif ( item.x>=5120 and item.y>=0 and item.x <= 5374 and item.y <= 254)
      Return 1;
Endif

Return 0;
EndFunction
