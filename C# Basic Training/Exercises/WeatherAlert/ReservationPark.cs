namespace ilrd
{

    public class ReservationPark
    {
        private readonly List<Traveler> _travelers = new List<Traveler>();

        public ReservationPark(WeatherAlerter forcaster_)
        {
            forcaster_.WeatherAlert += Notify;
        }

        public void RegisterAtPark(Traveler traveler_)
        {
            traveler_.InPark = true;
            _travelers.Add(traveler_);
        }

        [Obsolete]
        public void LeavePark(Traveler traveler_)
        {

        }

        public void Notify(WeatherEvent weatherEvent_)
        {
            foreach (Traveler traveler in _travelers)
            {
                traveler.React(weatherEvent_);
            }


#if DEBUG
            int howManyAreLeaving = 0;
#endif      
            //  TODO: this is not thread safe.
            for (int i = 0; i < _travelers.Count; ++i)
            {
                if (!_travelers[i].InPark)
                {
                    _travelers.Remove(_travelers[i]);
#if DEBUG
                    ++howManyAreLeaving;
#endif
                }
            }
#if DEBUG
            if (howManyAreLeaving > 0)
            {
                Console.WriteLine($"\n{howManyAreLeaving} groups just left. " +
                    $"there are now {_travelers.Count} Travelers left\n");
            }
            else
            {
                Console.WriteLine("\n nobody was scard off by this alert ...");
            }
#endif


        }
    }
}
