// 直接拒絕
if (tstart_time + shortest_path > tend_time)
{
    find = 0;
    cout << "(tstart_time + shortest_path > tend_time)" << endl;
    //* 抓出來的要放回去!
    basic_stations[tuser_start_station][tAC_bike_type[i]]
        .insertKey(target);
    cout << tuser_start_station << tAC_bike_type[i] << target.id << endl;
    break;
}
// v1把不符合條件的都集合起來，直到找出符合兩個條件的target
// v2後來發現不行，因為第二個條件跟bike無關，是時間和最短距離的問題

if ((target.returned_time > tstart_time))
{
    // todo 待優化，這邊先以最保守的方式來拿記憶體空間
    // 目的是，為了要暫時儲存不符合time資格的max node
    int tmp_idx = 0;
    BMNode *store_BMNode = new BMNode[bike_max_num];

    // cout << "target.returned_time: " << target.returned_time << endl;

    while (find && (target.returned_time > tstart_time))
    {
        cout << "here " << target.id << endl;
        cout << "test3" << endl;
        if (target.rental_price < 0)
        { // 抽到沒車了
            cout << "test3.5" << endl;
            find = 0;
            break;
        }
        else if (target.rental_count >= read_data.rental_limit)
        {
            cout << "reach rental limit!" << endl;
            // find = 0;
            continue;
        }

        store_BMNode[tmp_idx++] = target;
        target = basic_stations[tuser_start_station][tAC_bike_type[i]].extractMax();
    }

    cout << "test4" << endl;
    // 再一一insert回去
    for (int j = 1; j < tmp_idx; j++)
    {
        cout << "store_BMNode[j]:" << store_BMNode[j].id << endl;

        basic_stations[tuser_start_station][tAC_bike_type[i]].insertKey(store_BMNode[j]);
    }
    delete[] store_BMNode;

    tmp_idx = 0; // 歸零

    /*if (!find)
        continue;*/
}

//* 比較，不同bike_type之間最高的rental_price

if (find && tlen_AC > 1 && i == 0)
{
    compare.bike_type = target.bike_type;
    compare.id = target.id;
    compare.rental_count = target.rental_count;
    compare.rental_price = target.rental_price;
    compare.returned_time = target.returned_time;
    bike_type_index = i;

    cout << "compare id " << compare.id;
}
else if (find && i != 0 && target.rental_price > compare.rental_price)
{
    BMNode restored;
    restored.bike_type = compare.bike_type;
    restored.id = compare.id;
    restored.rental_count = compare.rental_count;
    restored.rental_price = compare.rental_price;
    restored.returned_time = compare.returned_time;

    cout << "restored-1 " << restored.id << endl;
    //* 把前面已經拿到，但發現rental_price比較小的bike放回去
    basic_stations[tuser_start_station][bike_type_index]
        .insertKey(restored);
    cout << "restored-2 " << restored.id << endl;
    cout << "test insert: " << basic_stations[tuser_start_station][bike_type_index].harr[0].id << endl;
    bike_type_index = i;
    BMNode *ptr = &compare;
    *ptr = target; // 把原本儲存compare的地址指向target
    cout << "ptr->id " << ptr->id << endl;
}
else if (!find && i != 0)
{
    target.bike_type = compare.bike_type;
    target.id = compare.id;
    target.rental_count = compare.rental_count;
    target.rental_price = compare.rental_price;
    target.returned_time = compare.returned_time;
}