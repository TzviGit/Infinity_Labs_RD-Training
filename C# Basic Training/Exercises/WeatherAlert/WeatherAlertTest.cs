namespace ilrd
{
    internal class WeatherAlertTest
    {
        public static void Main()
        {
            WeatherAlerter forcaster = new WeatherAlerter();
            ReservationPark reservationPark = new ReservationPark(forcaster);

            reservationPark.RegisterAtPark(new Family());
            reservationPark.RegisterAtPark(new YoungCouple());
            reservationPark.RegisterAtPark(new OlderGroup());

            forcaster.BroadcastAlert(new WeatherEvent(6));

            forcaster.BroadcastAlert(new WeatherEvent(1));
            forcaster.BroadcastAlert(new WeatherEvent(3));
            forcaster.BroadcastAlert(new WeatherEvent(9));
            forcaster.BroadcastAlert(new WeatherEvent(6));
        }
    }
}
