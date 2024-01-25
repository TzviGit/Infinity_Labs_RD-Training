namespace ilrd
{
    public class WeatherEvent
    {
        public int Intensity { get; private set; }

        public WeatherEvent(int intensity_)
        {
            Intensity = intensity_;
        }
    }

    public class WeatherAlerter
    {
        public event Action<WeatherEvent>? WeatherAlert;

        public void BroadcastAlert(WeatherEvent weather_)
        {
            WeatherAlert?.Invoke(weather_);
        }
    }

}
